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

Node asSequence(const Node &node) {
    if (node && node.IsSequence()) {
        return node;
    }

    Node seq(NodeType::Sequence);
    if (node && !node.IsNull()) {
        seq.push_back(node);
    }

    return seq;
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
        type = root["type"].as<string>();

    // Initial position
    if (root["position"]) {
        const auto &pos = root["position"];
        y_initial = pos["y_pos"].as<float>();
        x_initial = pos["x_pos"].as<float>();
        alpha_initial = pos["alpha"].as<float>();
        scale_initial = pos["scale"].as<float>();

        moveXpos(x_initial);
        this->y_pos += y_initial;
        alpha = alpha_initial;
        scale = scale_initial;
    }

    // Actions
    Node actionNodes = asSequence(root["action"]);
    for (const auto &action: actionNodes) {
        string name = action["name"].as<string>();
        string trigger = action["trigger"].as<string>();
        string from = action["from"].as<string>();
        string to = action["to"].as<string>();

        auto from_tokens = splitString(from, ", ");
        auto trigger_tokens = splitString(trigger, ", ");

        for (const auto &f: from_tokens)
            for (const auto &t: trigger_tokens)
                states[f][t] = name;

        next_state[name] = to;
    }

    // Animations
    Node animations = asSequence(root["animation"]);
    for (const auto &anim: animations) {
        string name = anim["name"].as<string>();
        bool is_combo = anim["combo"] ? anim["combo"].as<bool>() : false;
        bool is_continual = anim["continual"] ? anim["continual"].as<bool>() : false;
        bool show_hitbox = anim["show_hitbox"] ? anim["show_hitbox"].as<bool>() : false;

        animation = new Animation(is_combo, is_continual, show_hitbox);

        Node frames = asSequence(anim["frame"]);
        for (const auto &frame: frames) {
            string path = frame["path"].as<string>();
            float width = frame["width"].as<float>();
            float height = frame["height"].as<float>();
            float delay = frame["delay"].as<float>();

            defense = new Collision();
            offense = new Collision();
            actions = new Actions();

            // defense
            Node defs = asSequence(frame["defense"]);
            for (const auto &d: defs) {
                float x = d["x_pos"].as<float>();
                float y = d["y_pos"].as<float>();
                float w = d["width"].as<float>();
                float h = d["height"].as<float>();
                defense->addCollision(new CollisionBox(x, y, w, h, 0.f, 0.f, 1.f, 1.f));
            }

            Node offenses = asSequence(frame["offense"]);
            for (const auto &o: offenses) {
                float x = o["x_pos"].as<float>();
                float y = o["y_pos"].as<float>();
                float w = o["width"].as<float>();
                float h = o["height"].as<float>();
                offense->addCollision(new CollisionBox(x, y, w, h, 1.f, 0.f, 0.f, 1.f));
            }

            Node create_objects = asSequence(frame["create_object"]);
            for (const auto &obj: create_objects) {
                string path = obj["path"].as<string>();
                float x = obj["x_pos"].as<float>();
                float y = obj["y_pos"].as<float>();
                int index = obj["index"].as<int>();

                createObject(path);
                actions->addAction(new CreateObjectAction(path, x, y, index));
            }

            // collision
            Node collisions = asSequence(frame["collision"]);
            for (const auto &col: collisions) {
                string path = col["path"].as<string>();
                float x = col["x_pos"].as<float>();
                float y = col["y_pos"].as<float>();
                int index = col["index"].as<int>();
                string to = col["to"] ? col["to"].as<string>() : "";
                string to_opponent = col["opponent"] ? col["opponent"].as<string>() : "";

                createObject(path);
                actions->addAction(new CollisionAction(path, x, y, index, to, to_opponent));
            }

            if (frame["move"]) {
                float x = frame["move"]["x_vel"].as<float>();
                float y = frame["move"]["y_vel"].as<float>();
                actions->addAction(new MoveAction(x, y));
            }

            animation->addFrame(path, width, height, offense, defense, actions, delay);
        }

        animation_manager->add(name, animation);
    }
}
