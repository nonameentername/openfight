#include "json.h"
#include "player.h"
#include "moves.h"
#include "configuration.h"
#include <string>

using namespace std;

void Configuration::read() {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        return;
    }

    json root;
    try {
        file >> root;
    } catch (json::parse_error &e) {
        std::cerr << "Failed to parse JSON in file: " << file_name << std::endl;
        return;
    }
    auto readPlayerConfig = [&](const std::string &playerKey, int *config_keys, std::string *device_keys) {
        if (!root.contains(playerKey)) {
            std::cerr << "Missing or invalid '" << playerKey << "' array in JSON." << std::endl;
            return;
        }

        const auto &playerNode = root[playerKey];

        int j = 0;
        for (int i = CONFIG_KEY_UP; i <= CONFIG_KEY_Z; i++) {
            const std::string &keyName = config_name[i];
            if (playerNode.contains(keyName)) {
                const auto &item = playerNode[keyName];
                if (item.contains("key") && item.contains("device")) {
                    config_keys[j] = item["key"].get<int>();
                    device_keys[j] = item["device"].get<std::string>();
                    j++;
                }
            }
        }
    };

    readPlayerConfig("player_one", config_keys_one, device_keys_one);
    readPlayerConfig("player_two", config_keys_two, device_keys_two);
}

void Moves::initialize(std::string file_name) {
    std::ifstream ifs(file_name);
    if (!ifs) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        return;
    }

    json j;
    try {
        ifs >> j;
    } catch (...) {
        std::cerr << "Failed to parse JSON in file: " << file_name << std::endl;
        return;
    }

    if (!j.contains("moves") || !j["moves"].is_array()) {
        std::cerr << "Missing or invalid 'moves' array in JSON." << std::endl;
        return;
    }

    for (const auto &item: j["moves"]) {
        std::string keys = item.value("keys", "");
        std::string trigger = item.value("trigger", "");

        if (!keys.empty() && !trigger.empty()) {
            moves.push_back(new MoveSequence(keys, trigger));
        }
    }
}


void Player::initialize(string file_name, bool player_one, GLfloat x_pos, GLfloat y_pos) {
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    inverted = !player_one;
    state["INVERT"] = inverted;

    std::ifstream ifs(file_name);
    if (!ifs) {
        std::cerr << "Failed to open JSON file: " << file_name << std::endl;
        return;
    }

    json j;
    try {
        ifs >> j;
    } catch (...) {
        std::cerr << "Failed to parse JSON: " << file_name << std::endl;
        return;
    }

    auto root = j["character"];

    // type
    type = root.value("type", "");

    // position
    if (root.contains("position")) {
        const auto &pos = root["position"];
        y_initial = pos.value("y_pos", 0.0f);
        x_initial = pos.value("x_pos", 0.0f);
        alpha_initial = pos.value("alpha", 1.0f);
        scale_initial = pos.value("scale", 1.0f);

        moveXpos(x_initial);
        this->y_pos += y_initial;
        alpha = alpha_initial;
        scale = scale_initial;
    }

    // actions
    for (const auto &action: root["action"]) {
        string name = action.value("name", "");
        string trigger = action.value("trigger", "");
        string from = action.value("from", "");
        string to = action.value("to", "");

        auto from_tokens = splitString(from, ",");
        auto trigger_tokens = splitString(trigger, ",");

        for (const auto &f: from_tokens)
            for (const auto &t: trigger_tokens)
                states[f][t] = name;

        next_state[name] = to;
    }

    // animations
    for (const auto &anim: root["animation"]) {
        string name = anim.value("name", "");
        bool is_combo = anim.value("combo", false);
        bool is_continual = anim.value("continual", false);
        bool show_hitbox = anim.value("show_hitbox", false);

        Animation *animation = new Animation(is_combo, is_continual, show_hitbox);

        auto frames = anim["frame"].is_array() ? anim["frame"] : json::array({anim["frame"]});
        for (const auto &frame: frames) {
            string path = frame.value("path", "");
            float width = frame.value("width", 0.0f);
            float height = frame.value("height", 0.0f);
            float delay = frame.value("delay", 0.0f);

            Collision *defense = new Collision();
            Collision *offense = new Collision();
            Actions *actions = new Actions();

            // defense
            if (frame.contains("defense")) {
                auto defs = frame["defense"].is_array() ? frame["defense"] : json::array({frame["defense"]});
                for (const auto &d: defs) {
                    float x = d.value("x_pos", 0.0f);
                    float y = d.value("y_pos", 0.0f);
                    float w = d.value("width", 0.0f);
                    float h = d.value("height", 0.0f);
                    defense->addCollision(new CollisionBox(x, y, w, h, 0.f, 0.f, 1.f, 1.f));
                }
            }

            // offense
            if (frame.contains("offense")) {
                auto offs = frame["offense"].is_array() ? frame["offense"] : json::array({frame["offense"]});
                for (const auto &o: offs) {
                    float x = o.value("x_pos", 0.0f);
                    float y = o.value("y_pos", 0.0f);
                    float w = o.value("width", 0.0f);
                    float h = o.value("height", 0.0f);
                    offense->addCollision(new CollisionBox(x, y, w, h, 1.f, 0.f, 0.f, 1.f));
                }
            }

            // create_object
            if (frame.contains("create_object")) {
                auto objs = frame["create_object"].is_array()
                                ? frame["create_object"]
                                : json::array({frame["create_object"]});
                for (const auto &obj: objs) {
                    string path = obj.value("path", "");
                    float x = obj.value("x_pos", 0.0f);
                    float y = obj.value("y_pos", 0.0f);
                    int index = obj.value("index", 0);

                    createObject(path);
                    actions->addAction(new CreateObjectAction(path, x, y, index));
                }
            }

            // collision
            if (frame.contains("collision")) {
                auto cols = frame["collision"].is_array() ? frame["collision"] : json::array({frame["collision"]});
                for (const auto &col: cols) {
                    string path = col.value("path", "");
                    float x = col.value("x_pos", 0.0f);
                    float y = col.value("y_pos", 0.0f);
                    int index = col.value("index", 0);
                    string to = col.value("to", "");
                    string to_opponent = col.value("opponent", "");

                    createObject(path);
                    actions->addAction(new CollisionAction(path, x, y, index, to, to_opponent));
                }
            }

            // move
            if (frame.contains("move")) {
                float x_vel = frame["move"].value("x_vel", 0.0f);
                float y_vel = frame["move"].value("y_vel", 0.0f);
                actions->addAction(new MoveAction(x_vel, y_vel));
            }

            animation->addFrame(path, width, height, offense, defense, actions, delay);
        }

        animation_manager->add(name, animation);
    }
}
