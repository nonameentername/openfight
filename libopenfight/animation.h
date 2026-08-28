#ifndef OPEN_FIGHT_ANIMATION_H
#define OPEN_FIGHT_ANIMATION_H

#include "gl.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "actions.h"
#include "collision.h"
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
    GLfloat x_pos;
    GLfloat y_pos;
    GLfloat scale;
    void updateCollisions();

public:
    Animation(bool is_combo, bool is_continual, bool show_hitbox);
    Animation(const Animation &a);
    ~Animation();

    GLfloat getWidth();
    GLfloat getHeight();
    Collision *getOffense();
    Collision *getDefense();
    Actions *getActions();
    bool isCombo();
    bool isContinual();
    void addFrame(std::string file_name, GLfloat w, GLfloat h, Collision *o, Collision *d, Actions *a,
                  unsigned int delay);
    void update(GLfloat x, GLfloat y, GLfloat s, bool flip);
    bool nextFrame(bool &changed);
    void restart();
    void draw(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void draw(GLfloat alpha);
    void draw();
    void drawHitBox();
};

#endif
