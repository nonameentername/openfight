#ifndef _collision_h
#define _collision_h

#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include "collisionBox.h"

class Collision
{
   protected:
      std::vector<CollisionBox*> collisions;

   public:
      Collision();
      Collision(const Collision &c);
      ~Collision();

      void addCollision(CollisionBox *c);
      void update(GLfloat x, GLfloat y, bool inverted);
      bool overlaps(Collision *other);
      void draw();
};


#endif
