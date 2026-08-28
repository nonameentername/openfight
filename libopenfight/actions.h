#ifndef OPEN_FIGHT_ACTIONS_H
#define OPEN_FIGHT_ACTIONS_H

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

class Player;

class Action {
public:
    Action();
    virtual ~Action() = default;

    virtual void doAction(Player *player, Player *opponent) = 0;
};

class MoveAction : public Action {
protected:
    float x_vel;
    float y_vel;

public:
    MoveAction(float x_vel, float y_vel);
    virtual ~MoveAction() = default;

    void doAction(Player *player, Player *opponent);
};

class CreateObjectAction : public Action {
protected:
    std::string object;
    float x_pos;
    float y_pos;
    int index;

public:
    CreateObjectAction(std::string object, float x_pos, float y_pos, int index);
    virtual ~CreateObjectAction() = default;

    void doAction(Player *player, Player *opponent);
};

class CollisionAction : public Action {
protected:
    std::string object;
    float x_pos;
    float y_pos;
    int index;
    std::string to;
    std::string to_opponent;

public:
    CollisionAction(std::string object, float x_pos, float y_pos, int index, std::string to, std::string to_opponent);
    virtual ~CollisionAction() = default;

    void doAction(Player *player, Player *opponent);
};

class Actions {
protected:
    std::vector<Action *> actions;

public:
    Actions();
    Actions(const Actions &a);
    ~Actions();

    void addAction(Action *a);
    bool doActions(Player *player, Player *opponent, const std::type_info &type);
};

#endif
