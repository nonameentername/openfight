#ifndef OPEN_FIGHT_PLAYER_BRIDGE_H
#define OPEN_FIGHT_PLAYER_BRIDGE_H

#include "gl.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

#include "graphicsCore.h"
#include "player.h"
#include <SDL2/SDL_opengl.h>

class PlayerBridge {
private:
    Player *player_one;
    Player *player_two;

public:
    PlayerBridge();
    ~PlayerBridge();

    void initialize(Player *player_one, Player *player_two);
    void update();
};

#endif
