#ifndef _collision_h
#define _collision_h

#include <vector>
#include "gl.h"

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
