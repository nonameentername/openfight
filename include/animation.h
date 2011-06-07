#ifndef _animation_h
#define _animation_h

#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include "utilities.h"
#include "sprite.h"
#include "collision.h"
#include "actions.h"
#include "graphicsCore.h"


class Animation
{
   private:
      std::vector<Sprite*>    sprites;
      std::vector<Collision*> offense;
      std::vector<Collision*> defense;
      std::vector<Actions*>    action;
      std::vector<uint>       delays;
      uint current;
      uint current_delay;
      bool is_combo;
      bool is_continual;
      bool inverted;
      GLfloat x_pos;
      GLfloat y_pos;
      GLfloat scale;
      void updateCollisions();

   public:
      Animation(bool is_combo, bool is_continual);
      Animation(const Animation &a);
      ~Animation();

      GLfloat getWidth();
      GLfloat getHeight();
      Collision *getOffense();
      Collision *getDefense();
      Actions   *getActions();
      bool isCombo();
      bool isContinual();
      void addFrame(std::string file_name, GLfloat w, GLfloat h, Collision *o, Collision *d, Actions *a, uint delay);
      void update(GLfloat x, GLfloat y, GLfloat s, bool flip);
      bool nextFrame(bool &changed);
      void restart();
      void draw(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
      void draw(GLfloat alpha);
      void draw();
};


#endif
