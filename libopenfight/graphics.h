#ifndef OPEN_FIGHT_GRAPHICS_H
#define OPEN_FIGHT_GRAPHICS_H

#include "gl.h"

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

#endif
