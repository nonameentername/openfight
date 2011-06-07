#ifndef _graphics_core_h
#define _graphics_core_h

#include "camera.h"
#include "graphics.h"
#include "objectManager.h"
#include "textureManager.h"

class Player;

extern ObjectManager<Player> *object_manager;
extern TextureManager *texture_manager;
extern Camera   *camera;
extern Graphics *graphics;


#endif
