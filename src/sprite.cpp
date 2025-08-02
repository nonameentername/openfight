// clang-format off
#include "graphicsCore.h"
#include "sprite.h"
// clang-format on

Sprite::Sprite() {
    has_mask = false;
}

Sprite::~Sprite() {
}

GLfloat Sprite::getWidth() {
    return width;
}

GLfloat Sprite::getHeight() {
    return height;
}

void Sprite::setTexture(GLuint texture, GLfloat w, GLfloat h) {
    this->texture = texture;
    width = w;
    height = h;
}

void Sprite::setMask(GLuint mask) {
    has_mask = true;
    this->mask = mask;
}

void Sprite::draw(GLfloat x, GLfloat y, GLfloat scale, bool flip, GLfloat red, GLfloat green, GLfloat blue,
                  GLfloat alpha) {
    glPushMatrix();

    int tX = width / 2.0f * scale;
    int tY = height / 2.0f * scale;

    if (flip)
        tX = -tX;

    glTranslatef(camera->getXposition() + x, camera->getYposition() + y, camera->getZposition());
    // glRotatef(rotate, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    if (has_mask && alpha == 1) {
        glBlendFunc(GL_DST_COLOR, GL_ZERO);
        glBindTexture(GL_TEXTURE_2D, mask);

        glBegin(GL_QUADS);
        glColor4f(red, green, blue, alpha);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3i(-tX, -tY, 0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3i(-tX, tY, 0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3i(tX, tY, 0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3i(tX, -tY, 0);
        glEnd();
    }

    glBlendFunc(GL_ONE, GL_ONE);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glColor4f(red, green, blue, alpha);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3i(-tX, -tY, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3i(-tX, tY, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3i(tX, tY, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3i(tX, -tY, 0);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
}

void Sprite::draw(GLfloat x, GLfloat y, GLfloat scale, bool flip, GLfloat alpha) {
    draw(x, y, scale, flip, alpha, alpha, alpha, alpha);
}

void Sprite::draw(GLfloat x, GLfloat y, GLfloat scale, bool flip) {
    draw(x, y, scale, flip, 1.0, 1.0, 1.0, 0.0);
}
