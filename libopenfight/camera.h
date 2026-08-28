#ifndef OPEN_FIGHT_CAMERA_H
#define OPEN_FIGHT_CAMERA_H

#include "gl.h"

class Camera {
private:
    GLfloat x_pos;
    GLfloat y_pos;
    GLfloat z_pos;

public:
    Camera();
    ~Camera();

    void move(GLfloat x, GLfloat y, GLfloat z);
    void setZoom(GLfloat z);
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    GLfloat getXposition();
    GLfloat getYposition();
    GLfloat getZposition();
};

#endif
