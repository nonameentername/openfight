#ifndef _utilities_h
#define _utilities_h

#include <string>
#include <vector>
#include <SDL_timer.h>

extern const float GAME_TIME_UPDATE_FREQ;

extern float getGameTime();

extern bool  updateGame(float last_update);

std::vector<std::string> splitString(std::string str, std::string delimeters);

#endif
