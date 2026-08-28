#ifndef OPEN_FIGHT_CAMERA_H
#define OPEN_FIGHT_CAMERA_H

class Camera {
private:
    float x_pos;
    float y_pos;
    float z_pos;

public:
    Camera();
    ~Camera();

    void move(float x, float y, float z);
    void setZoom(float z);
    void setPosition(float x, float y, float z);
    float getXposition();
    float getYposition();
    float getZposition();
};

#endif
