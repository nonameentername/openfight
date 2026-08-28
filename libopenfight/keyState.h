#ifndef OPEN_FIGHT_KEY_STATE_H
#define OPEN_FIGHT_KEY_STATE_H

#include "input.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <list>
#include <string>

class KeyState {
private:
    int key;
    int time_held;
    bool pressed;
    bool held;

public:
    KeyState(int key, bool pressed);
    KeyState(int key, int time_held, bool pressed, bool held);
    ~KeyState();

    bool operator==(const KeyState *c) const;
    void setTimeHeld(int time_held);
    void setHeld(bool held);
    int getKey();
    bool keyPressed();
};

#endif
