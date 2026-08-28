#include "utilities.h"

using namespace std;

const float GAME_TIME_UPDATE_FREQ = 10;

float getGameTime() {
    return SDL_GetTicks();
}

bool updateGame(float last_update) {
    return (getGameTime() - last_update) > GAME_TIME_UPDATE_FREQ;
}

vector<string> splitString(string str, string delimeters) {
    vector<string> result;
    char *i;

    vector<char> buffer(str.begin(), str.end());
    buffer.push_back('\0');

    i = strtok(&buffer[0], delimeters.c_str());

    while (i != NULL) {
        result.push_back(i);
        i = strtok(NULL, delimeters.c_str());
    }

    return result;
}
