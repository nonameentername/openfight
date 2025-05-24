#ifndef _sprite_h
#define _sprite_h

#include <SDL2/SDL_opengl.h>

class Sprite
{
   private:
      GLuint texture;
      GLuint mask;
      bool has_mask;
      GLfloat width;
      GLfloat height;
   public:
      Sprite();
      ~Sprite();

      GLfloat getWidth();
      GLfloat getHeight();
      void setTexture(GLuint texture, GLfloat w, GLfloat h);
      void setMask(GLuint mask);
      void draw(GLfloat x, GLfloat y, GLfloat scale, bool flip, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
      void draw(GLfloat x, GLfloat y, GLfloat scale, bool flip, GLfloat alpha);
      void draw(GLfloat x, GLfloat y, GLfloat scale, bool flip);
};


#endif
