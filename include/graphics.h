#ifndef _graphics_h
#define _graphics_h

#include <SDL/SDL_opengl.h>

class Graphics
{
   private:
      bool global_loaded;
      int width;
      int height;
   public:
      Graphics();
      ~Graphics();
      void initialize(int w, int h);
      void resizeWindow(int w, int h);
      int  getScreenWidth();
      int  getScreenHeight();
};


#endif
