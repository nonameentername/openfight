#ifndef OPEN_FIGHT_ACTOR_H
#define OPEN_FIGHT_ACTOR_H

#include <iostream>
#include <string>

class Actor {
protected:
    float x, y;

public:
    Actor();
    virtual void initialize();
    virtual void update();
};

#endif
