#include "camera.h"

Camera::Camera() {
    x_pos = 0;
    y_pos = 0;
}

Camera::~Camera() {
}

void Camera::move(float x, float y, float z) {
    x_pos += x;
    y_pos += y;
    z_pos += z;
}

void Camera::setZoom(float z) {
    z_pos = z;
}

void Camera::setPosition(float x, float y, float z) {
    x_pos = x;
    y_pos = y;
    z_pos = z;
}

float Camera::getXposition() {
    return x_pos;
}

float Camera::getYposition() {
    return y_pos;
}

float Camera::getZposition() {
    return z_pos;
}
