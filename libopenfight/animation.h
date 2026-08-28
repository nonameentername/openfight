#ifndef OPEN_FIGHT_ANIMATION_H
#define OPEN_FIGHT_ANIMATION_H

#include <string>
#include <vector>

#include "actions.h"
#include "collision.h"
#include "renderBackend.h"
#include "sprite.h"

class Animation {
private:
    std::vector<Sprite *> sprites;
    std::vector<Collision *> offense;
    std::vector<Collision *> defense;
    std::vector<Actions *> action;
    std::vector<unsigned int> delays;
    unsigned int current;
    unsigned int current_delay;
    bool is_combo;
    bool is_continual;
    bool show_hitbox;
    bool inverted;
    float x_pos;
    float y_pos;
    float scale;
    void updateCollisions();

public:
    Animation(bool is_combo, bool is_continual, bool show_hitbox);
    Animation(const Animation &a);
    ~Animation();

    float getWidth();
    float getHeight();
    Collision *getOffense();
    Collision *getDefense();
    Actions *getActions();
    bool isCombo();
    bool isContinual();
    void addFrame(std::string file_name, float w, float h, Collision *o, Collision *d, Actions *a, unsigned int delay);
    void update(float x, float y, float s, bool flip);
    bool nextFrame(bool &changed);
    void restart();
    void draw(RenderBackend &renderer, float red, float green, float blue, float alpha);
    void draw(RenderBackend &renderer, float alpha);
    void draw(RenderBackend &renderer);
    void drawHitBox(RenderBackend &renderer);
};

#endif
