#ifndef OPEN_FIGHT_SPRITE_H
#define OPEN_FIGHT_SPRITE_H

#include "renderBackend.h"

class Sprite {
private:
    unsigned int texture;
    unsigned int mask;
    bool has_mask;
    float width;
    float height;

public:
    Sprite();
    ~Sprite();

    float getWidth();
    float getHeight();
    void setTexture(unsigned int texture, float w, float h);
    void setMask(unsigned int mask);
    void draw(RenderBackend &renderer, float x, float y, float scale, bool flip, float red, float green, float blue,
              float alpha);
    void draw(RenderBackend &renderer, float x, float y, float scale, bool flip, float alpha);
    void draw(RenderBackend &renderer, float x, float y, float scale, bool flip);
};

#endif
