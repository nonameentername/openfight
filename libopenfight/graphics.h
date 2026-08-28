#ifndef OPEN_FIGHT_GRAPHICS_H
#define OPEN_FIGHT_GRAPHICS_H

#include "gl.h"

#include <SDL2/SDL_opengl.h>

class Graphics {
private:
    bool global_loaded;
    int width;
    int height;

public:
    Graphics();
    ~Graphics();
    void initialize(int w, int h);
    void resizeWindow(int w, int h);
    int getScreenWidth();
    int getScreenHeight();
};

const char *SDL_GetCompiledVersion();

#endif
