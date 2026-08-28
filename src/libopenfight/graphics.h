#ifndef OPEN_FIGHT_GRAPHICS_H
#define OPEN_FIGHT_GRAPHICS_H

#include "renderBackend.h"

class Graphics {
private:
    bool global_loaded;
    int width;
    int height;

public:
    Graphics();
    ~Graphics();
    void initialize(int w, int h, RenderBackend &renderer);
    void resizeWindow(int w, int h, RenderBackend &renderer);
    int getScreenWidth();
    int getScreenHeight();
};

#endif
