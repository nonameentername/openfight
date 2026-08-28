// clang-format off
#include "graphicsCore.h"
#include "sprite.h"
// clang-format on

Sprite::Sprite() {
    has_mask = false;
}

Sprite::~Sprite() {
}

float Sprite::getWidth() {
    return width;
}

float Sprite::getHeight() {
    return height;
}

void Sprite::setTexture(unsigned int texture, float w, float h) {
    this->texture = texture;
    width = w;
    height = h;
}

void Sprite::setMask(unsigned int mask) {
    has_mask = true;
    this->mask = mask;
}

void Sprite::draw(RenderBackend &renderer, float x, float y, float scale, bool flip, float red, float green, float blue,
                  float alpha) {
    RenderColor color = {red, green, blue, alpha};
    renderer.drawSprite(texture, mask, has_mask, width, height, x, y, scale, flip, color);
}

void Sprite::draw(RenderBackend &renderer, float x, float y, float scale, bool flip, float alpha) {
    draw(renderer, x, y, scale, flip, alpha, alpha, alpha, alpha);
}

void Sprite::draw(RenderBackend &renderer, float x, float y, float scale, bool flip) {
    draw(renderer, x, y, scale, flip, 1.0, 1.0, 1.0, 1.0);
}
