#ifndef OPEN_FIGHT_INPUT_H
#define OPEN_FIGHT_INPUT_H

#include <string>
#include <vector>

enum Keys {
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_MAX
};

struct KeyStruct {
    std::string config_device[KEY_MAX];
    int config_keys[KEY_MAX];
    bool keys[KEY_MAX];
};

inline std::string getKeyName(int key) {
    static const char *key_names[KEY_MAX] = {"U", "D", "B", "F", "a", "b", "c", "x", "y", "z"};
    return key_names[key];
}

#endif
