#ifndef OPEN_FIGHT_GRAPHICS_CORE_H
#define OPEN_FIGHT_GRAPHICS_CORE_H

#include "camera.h"
#include "graphics.h"
#include "objectManager.h"
#include "textureManager.h"

class Player;

extern ObjectManager<Player> *object_manager;
extern TextureManager *texture_manager;
extern Camera *camera;
extern Graphics *graphics;

#endif
