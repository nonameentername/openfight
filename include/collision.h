#ifndef OPEN_FIGHT_COLLISION_H
#define OPEN_FIGHT_COLLISION_H

#include "gl.h"
#include <vector>

#include "collisionBox.h"

class Collision {
protected:
    std::vector<CollisionBox *> collisions;

public:
    Collision();
    Collision(const Collision &c);
    ~Collision();

    void addCollision(CollisionBox *c);
    void update(GLfloat x, GLfloat y, bool inverted);
    bool overlaps(Collision *other);
    void draw();
};

#endif
