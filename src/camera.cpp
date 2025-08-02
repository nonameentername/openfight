#include "camera.h"

Camera::Camera() {
    x_pos = 0;
    y_pos = 0;
}

Camera::~Camera() {
}

void Camera::move(GLfloat x, GLfloat y, GLfloat z) {
    x_pos += x;
    y_pos += y;
    z_pos += z;
}

void Camera::setZoom(GLfloat z) {
    z_pos = z;
}

void Camera::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    x_pos = x;
    y_pos = y;
    z_pos = z;
}

GLfloat Camera::getXposition() {
    return x_pos;
}

GLfloat Camera::getYposition() {
    return y_pos;
}

GLfloat Camera::getZposition() {
    return z_pos;
}
