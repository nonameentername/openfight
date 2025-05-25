#ifndef _player_bridge_h
#define _player_bridge_h

#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL_opengl.h>
#include "player.h"
#include "graphicsCore.h"

class PlayerBridge
{
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
