#ifndef _utilities_h
#define _utilities_h

#include <iostream>
#include <string>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <SDL2/SDL.h>
#include "gl.h"

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

extern const float GAME_TIME_UPDATE_FREQ;

extern float getGameTime();

extern bool  updateGame(float last_update);

extern xmlNode *findChild(xmlNode *parent, std::string name);

extern std::vector<xmlNode*> findChildren(xmlNode *parent, std::string name);

template<class T> T getProperty(xmlNode *node, std::string name);

template<class T> T getContent(xmlNode *node);

std::vector<std::string> splitString(std::string str, std::string delimeters);

#endif
