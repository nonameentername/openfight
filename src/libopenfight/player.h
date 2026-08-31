#ifndef OPEN_FIGHT_PLAYER_H
#define OPEN_FIGHT_PLAYER_H

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>

#include "actions.h"
#include "animation.h"
#include "collision.h"
#include "fileReader.h"
#include "graphicsCore.h"
#include "objectManager.h"
#include "utilities.h"

class Player {
private:
    float x_pos;
    float y_pos;
    float alpha;
    float scale;

    float y_initial;
    float x_initial;
    float alpha_initial;
    float scale_initial;

    std::string type;
    bool inverted;
    bool is_hurt;
    int index;

    std::string current_state;
    std::string previous_state;

    std::map<std::string, std::map<std::string, std::string>> states;
    std::map<std::string, std::string> next_state;
    ObjectManager<Animation> *animation_manager;

    Player *opponent;
    ObjectManager<Player> *objects;
    ReadTextFileCallback yaml_loader;

    void setCurrentState(std::string state);
    void checkCollisions(float action_x_vel);

public:
    Player();
    Player(const Player &p);
    ~Player();

    std::map<std::string, bool> state;

    bool operator<(const Player &p) const;
    float getXpos();
    void moveXpos(float value);
    bool isInverted();
    bool isHurt();
    Collision *getOffense();
    Collision *getDefense();

    void initialize(std::string file_name, bool player_one, float x_pos, float y_pos,
                    const ReadTextFileCallback &read_text_file = ReadTextFileCallback());
    void setOpponent(Player *opponent);
    bool updateState(std::string command);
    bool updateState(std::string command, bool from_opponent);
    bool update();
    void doAction(const std::type_info &type);
    void move(float x, float y);
    void setLocation(float x, float y);
    void createObject(std::string object);
    void copyObject(std::string object, float x, float y, int index);
    void draw(RenderBackend &renderer);
};

#endif
