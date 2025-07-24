#include "yaml.h"
#include "player.h"
#include "moves.h"
#include "configuration.h"

void Configuration::read() {
    ifstream file(file_name);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << file_name << endl;
        return;
    }

    Node root;
    try {
        root = Load(file);
    } catch (const ParserException &e) {
        cerr << "Failed to parse YAML in file: " << file_name << endl
                << e.what() << endl;
        return;
    }

    if (!root["configuration"] || !root["configuration"].IsMap()) {
        cerr << "Missing or invalid 'configuration' node in YAML." << endl;
        return;
    }

    const Node &configNode = root["configuration"];

    auto readPlayerConfig = [&](const string &playerKey, int *config_keys, string *device_keys) {
        if (!configNode[playerKey] || !configNode[playerKey].IsMap()) {
            cerr << "Missing or invalid '" << playerKey << "' node in 'configuration'." << endl;
            return;
        }

        const Node &playerNode = configNode[playerKey];

        int j = 0;
        for (int i = CONFIG_KEY_UP; i <= CONFIG_KEY_Z; i++) {
            const string &keyName = config_name[i];
            if (playerNode[keyName] && playerNode[keyName].IsMap()) {
                const Node &item = playerNode[keyName];
                if (item["key"] && item["device"]) {
                    config_keys[j] = item["key"].as<int>();
                    device_keys[j] = item["device"].as<string>();
                    j++;
                }
            }
        }
    };

    readPlayerConfig("player_one", config_keys_one, device_keys_one);
    readPlayerConfig("player_two", config_keys_two, device_keys_two);
}


void Moves::initialize(string file_name) {
    ifstream ifs(file_name);
    if (!ifs) {
        cerr << "Failed to open file: " << file_name << endl;
        return;
    }

    Node root;
    try {
        root = Load(ifs);
    } catch (const ParserException &e) {
        cerr << "Failed to parse YAML in file: " << file_name << endl
                << e.what() << endl;
        return;
    }

    if (!root["moves"] || !root["moves"]["move"] || !root["moves"]["move"].IsSequence()) {
        cerr << "Missing or invalid 'moves > move' array in YAML." << endl;
        return;
    }

    const Node &moveList = root["moves"]["move"];

    for (const auto &item: moveList) {
        auto keys = item["keys"].as<string>();
        auto trigger = item["trigger"].as<string>();

        if (!keys.empty() && !trigger.empty()) {
            moves.push_back(new MoveSequence(keys, trigger));
        }
    }
}

inline Node asSequence(const Node &node) {
    if (node && node.IsSequence()) return node;
    Node seq(NodeType::Sequence);
    if (node && !node.IsNull()) seq.push_back(node);
    return seq;
}

inline float getFloat(const Node &node, const string &key, float defaultValue = 0.0f) {
    return node[key] ? node[key].as<float>() : defaultValue;
}

inline string getString(const Node &node, const string &key, const string &defaultValue = "") {
    return node[key] ? node[key].as<string>() : defaultValue;
}

inline void addCollisions(const Node &node, Collision *coll, float r, float g, float b, float alpha) {
    for (const auto &c: asSequence(node)) {
        auto x = getFloat(c, "x_pos");
        auto y = getFloat(c, "y_pos");
        auto w = getFloat(c, "width");
        auto h = getFloat(c, "height");
        coll->addCollision(new CollisionBox(x, y, w, h, r, g, b, alpha));
    }
}

void Player::initialize(string file_name, bool player_one, GLfloat x_pos, GLfloat y_pos) {
    Animation *animation;
    Actions *actions;
    Collision *defense;
    Collision *offense;

    // Handle player orientation
    state["INVERT"] = !player_one;
    inverted = !player_one;

    this->x_pos = x_pos;
    this->y_pos = y_pos;

    // Load YAML and access root node "character"
    Node root_yaml = LoadFile(file_name);
    Node root = root_yaml["character"];
    if (!root) {
        cerr << "Missing root node 'character' in YAML file: " << file_name << endl;
        return;
    }

    // Character type
    if (root["type"])
        type = getString(root, "type");

    // Initial position
    if (root["position"]) {
        const auto &pos = root["position"];
        y_initial = getFloat(pos, "y_pos");
        x_initial = getFloat(pos, "x_pos");
        alpha_initial = getFloat(pos, "alpha");
        scale_initial = getFloat(pos, "scale");

        moveXpos(x_initial);
        this->y_pos += y_initial;
        alpha = alpha_initial;
        scale = scale_initial;
    }

    // Actions
    for (const auto &action: asSequence(root["action"])) {
        auto name = getString(action, "name");
        auto trigger = getString(action, "trigger");
        auto from = getString(action, "from");
        auto to = getString(action, "to");

        auto from_tokens = splitString(from, ", ");
        auto trigger_tokens = splitString(trigger, ", ");

        for (const auto &f: from_tokens)
            for (const auto &t: trigger_tokens)
                states[f][t] = name;

        next_state[name] = to;
    }

    // Animations
    for (const auto &anim: asSequence(root["animation"])) {
        auto name = getString(anim, "name");
        auto is_combo = anim["combo"] ? anim["combo"].as<bool>() : false;
        auto is_continual = anim["continual"] ? anim["continual"].as<bool>() : false;
        auto show_hitbox = anim["show_hitbox"] ? anim["show_hitbox"].as<bool>() : false;

        animation = new Animation(is_combo, is_continual, show_hitbox);

        // Frames
        for (const auto &frame: asSequence(anim["frame"])) {
            auto path = getString(frame, "path");
            auto width = getFloat(frame, "width");
            auto height = getFloat(frame, "height");
            auto delay = getFloat(frame, "delay");

            defense = new Collision();
            offense = new Collision();
            actions = new Actions();

            // defense
            for (const auto &d: asSequence(frame["defense"])) {
                addCollisions(d, defense, 0.f, 0.f, 1.f, 1.f);
            }

            for (const auto &o: asSequence(frame["offense"])) {
                addCollisions(o, offense, 1.f, 0.f, 0.f, 1.f);
            }

            for (const auto &obj: asSequence(frame["create_object"])) {
                auto path = getString(obj, "path");
                auto x = getFloat(obj, "x_pos");
                auto y = getFloat(obj, "y_pos");
                auto index = obj["index"].as<int>();

                createObject(path);
                actions->addAction(new CreateObjectAction(path, x, y, index));
            }

            // collision
            for (const auto &col: asSequence(frame["collision"])) {
                auto path = getString(col, "path");
                auto x = getFloat(col, "x_pos");
                auto y = getFloat(col, "y_pos");
                auto index = col["index"].as<int>();
                auto to = getString(col, "to");
                auto to_opponent = getString(col, "opponent");

                createObject(path);
                actions->addAction(new CollisionAction(path, x, y, index, to, to_opponent));
            }

            if (frame["move"]) {
                auto move = frame["move"];
                auto x = getFloat(move, "x_vel");
                auto y = getFloat(move, "y_vel");
                actions->addAction(new MoveAction(x, y));
            }

            animation->addFrame(path, width, height, offense, defense, actions, delay);
        }

        animation_manager->add(name, animation);
    }
}
