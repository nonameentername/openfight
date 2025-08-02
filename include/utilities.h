#ifndef OPEN_FIGHT_UTILITIES_H
#define OPEN_FIGHT_UTILITIES_H

#include <SDL_timer.h>
#include <string>
#include <vector>

extern const float GAME_TIME_UPDATE_FREQ;

extern float getGameTime();

extern bool updateGame(float last_update);

std::vector<std::string> splitString(std::string str, std::string delimeters);

#endif
