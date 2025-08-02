#ifndef OPEN_FIGHT_CONFIGURATION_H
#define OPEN_FIGHT_CONFIGURATION_H

#include "input.h"
#include "utilities.h"
#include <list>
#include <map>
#include <string>
#include <typeinfo>

enum Config {
    CONFIG_PLAYER_ONE,
    CONFIG_PLAYER_TWO,
    CONFIG_KEY_UP,
    CONFIG_KEY_DOWN,
    CONFIG_KEY_LEFT,
    CONFIG_KEY_RIGHT,
    CONFIG_KEY_A,
    CONFIG_KEY_B,
    CONFIG_KEY_C,
    CONFIG_KEY_X,
    CONFIG_KEY_Y,
    CONFIG_KEY_Z,
    CONFIG_MAX
};

class Configuration {
private:
    std::string file_name;

    static std::string config_name[CONFIG_MAX];
    int config_keys_one[KEY_MAX];
    int config_keys_two[KEY_MAX];
    std::string device_keys_one[KEY_MAX];
    std::string device_keys_two[KEY_MAX];

public:
    Configuration(std::string file_name);
    ~Configuration();

    void read();
    void write();

    int *getConfigKeys(bool player_one);
    std::string *getConfigDevice(bool player_one);
};

#endif
