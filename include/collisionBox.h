#ifndef _collision_box_h
#define _collision_box_h

#include <SDL/SDL_opengl.h>
#include "graphicsCore.h"

class CollisionBox
{
   private:
      GLfloat x_pos;
      GLfloat y_pos;
      GLfloat width;
      GLfloat height;
      GLfloat red;
      GLfloat blue;
      GLfloat green;
      GLfloat alpha;

      bool inverted;

      GLfloat x;
      GLfloat y;
   public:
      CollisionBox(GLfloat x_pos, GLfloat y_pos, GLfloat width, GLfloat height);
      CollisionBox(GLfloat x_pos, GLfloat y_pos, GLfloat width, GLfloat height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
      ~CollisionBox();

      GLfloat x1();
      GLfloat x2();
      GLfloat y1();
      GLfloat y2();

      void update(GLfloat x, GLfloat y, bool inverted);
      bool overlaps(CollisionBox *other);
      void draw();

};


#endif
