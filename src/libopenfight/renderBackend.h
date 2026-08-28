#ifndef OPEN_FIGHT_RENDER_BACKEND_H
#define OPEN_FIGHT_RENDER_BACKEND_H

#include <string>

struct RenderColor {
    float red;
    float green;
    float blue;
    float alpha;
};

class RenderBackend {
public:
    virtual ~RenderBackend() = default;

    virtual void initialize(int width, int height) = 0;
    virtual void resizeWindow(int width, int height) = 0;
    virtual void beginFrame() = 0;
    virtual unsigned int loadTexture(const std::string &file_name, bool mipmap, bool masking) = 0;
    virtual void releaseTexture(unsigned int texture) = 0;
    virtual void drawSprite(unsigned int texture, unsigned int mask, bool has_mask, float width, float height, float x,
                            float y, float scale, bool flip, RenderColor color) = 0;
    virtual void drawCollisionBox(float x, float y, float x_pos, float y_pos, float width, float height, bool inverted,
                                  RenderColor color) = 0;
};

#endif
