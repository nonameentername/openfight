#include "collisionBox.h"

CollisionBox::CollisionBox(float x_pos, float y_pos, float width, float height) {
    CollisionBox(x_pos, y_pos, width, height, 1.f, 1.f, 1.f, 0.f);
}

CollisionBox::CollisionBox(float x_pos, float y_pos, float width, float height, float red, float green, float blue,
                           float alpha) {
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    this->width = width;
    this->height = height;
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
}

CollisionBox::~CollisionBox() {
}

void CollisionBox::update(float x, float y, bool inverted) {
    this->inverted = inverted;
    this->x = x;
    this->y = y;
}

float CollisionBox::x1() {
    float x_posi = inverted ? -(x_pos + width) : x_pos;
    return x + x_posi;
}

float CollisionBox::x2() {
    float x_posi = inverted ? -(x_pos + width) : (x_pos);
    return x + x_posi + width;
}

float CollisionBox::y1() {
    return y + y_pos;
}

float CollisionBox::y2() {
    return y + y_pos + height;
}

bool CollisionBox::overlaps(CollisionBox *other) {
    if (other != NULL) {
        return other->x1() < this->x2() && other->x2() > this->x1() && other->y1() < this->y2() &&
               other->y2() > this->y1();
    }

    return false;
}

void CollisionBox::draw(RenderBackend &renderer) {
    RenderColor color = {red, green, blue, alpha};
    renderer.drawCollisionBox(x, y, x_pos, y_pos, width, height, inverted, color);
}
