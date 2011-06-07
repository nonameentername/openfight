#ifndef _camera_h
#define _camera_h

#include <SDL/SDL_opengl.h>

class Camera
{
   private:
      GLfloat x_pos;
      GLfloat y_pos;
      GLfloat z_pos;

   public:
      Camera();
      ~Camera();

      void move(GLfloat x, GLfloat y, GLfloat z);
      void setZoom(GLfloat z);
      void setPosition(GLfloat x, GLfloat y, GLfloat z);
      GLfloat getXposition();
      GLfloat getYposition();
      GLfloat getZposition();
};


#endif
