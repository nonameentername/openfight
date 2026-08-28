#ifndef OPEN_FIGHT_SDL_GL_RENDER_BACKEND_H
#define OPEN_FIGHT_SDL_GL_RENDER_BACKEND_H

#include "renderBackend.h"
#include <SDL2/SDL.h>
#include <cstdint>

struct SDL_Surface;

class SdlGlRenderBackend : public RenderBackend {
public:
    void initialize(int width, int height) override;
    void resizeWindow(int width, int height) override;
    void beginFrame() override;
    unsigned int loadTexture(const std::string &file_name, bool mipmap, bool masking) override;
    void releaseTexture(unsigned int texture) override;
    void drawSprite(unsigned int texture, unsigned int mask, bool has_mask, float width, float height, float x, float y,
                    float scale, bool flip, RenderColor color) override;
    void drawCollisionBox(float x, float y, float x_pos, float y_pos, float width, float height, bool inverted,
                          RenderColor color) override;

private:
    uint32_t getPixel(SDL_Surface *surface, int x, int y);
    void setPixel(SDL_Surface *surface, int x, int y, uint32_t pixel);
    void createMask(SDL_Surface *image);
};

#endif
