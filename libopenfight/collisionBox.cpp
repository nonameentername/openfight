#include "collisionBox.h"

CollisionBox::CollisionBox(GLfloat x_pos, GLfloat y_pos, GLfloat width, GLfloat height) {
    CollisionBox(x_pos, y_pos, width, height, 1.f, 1.f, 1.f, 0.f);
}

CollisionBox::CollisionBox(GLfloat x_pos, GLfloat y_pos, GLfloat width, GLfloat height, GLfloat red, GLfloat green,
                           GLfloat blue, GLfloat alpha) {
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

void CollisionBox::update(GLfloat x, GLfloat y, bool inverted) {
    this->inverted = inverted;
    this->x = x;
    this->y = y;
}

GLfloat CollisionBox::x1() {
    GLfloat x_posi = inverted ? -(x_pos + width) : x_pos;
    return x + x_posi;
}

GLfloat CollisionBox::x2() {
    GLfloat x_posi = inverted ? -(x_pos + width) : (x_pos);
    return x + x_posi + width;
}

GLfloat CollisionBox::y1() {
    return y + y_pos;
}

GLfloat CollisionBox::y2() {
    return y + y_pos + height;
}

bool CollisionBox::overlaps(CollisionBox *other) {
    if (other != NULL) {
        return other->x1() < this->x2() && other->x2() > this->x1() && other->y1() < this->y2() &&
               other->y2() > this->y1();
    }

    return false;
}

void CollisionBox::draw() {
    glPushMatrix();

    int tX = width / 2.0f;
    int tY = height / 2.0f;

    GLfloat tXi = inverted ? -tX : tX;
    GLfloat x_posi = inverted ? -x_pos : x_pos;

    glTranslatef(camera->getXposition() + x + x_posi + tXi, camera->getYposition() + y + y_pos + tY,
                 camera->getZposition());
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glLineWidth(2);
    glColor4f(red, green, blue, alpha);

    glBegin(GL_LINE_LOOP);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3i(-tX, -tY, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3i(-tX, tY, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3i(tX, tY, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3i(tX, -tY, 0);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glPopMatrix();
}
