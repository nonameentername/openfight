#ifndef OPEN_FIGHT_COLLISION_BOX_H
#define OPEN_FIGHT_COLLISION_BOX_H

#include "graphicsCore.h"
#include "renderBackend.h"

class CollisionBox {
private:
    float x_pos;
    float y_pos;
    float width;
    float height;
    float red;
    float blue;
    float green;
    float alpha;

    bool inverted;

    float x;
    float y;

public:
    CollisionBox(float x_pos, float y_pos, float width, float height);
    CollisionBox(float x_pos, float y_pos, float width, float height, float red, float green, float blue, float alpha);
    ~CollisionBox();

    float x1();
    float x2();
    float y1();
    float y2();

    void update(float x, float y, bool inverted);
    bool overlaps(CollisionBox *other);
    void draw(RenderBackend &renderer);
};

#endif
