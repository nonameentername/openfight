#include "utilities.h"
#include <chrono>
#include <cstring>

using namespace std;

const float GAME_TIME_UPDATE_FREQ = 10;

float getGameTime() {
    using namespace std::chrono;

    static const steady_clock::time_point start = steady_clock::now();
    return duration_cast<duration<float, std::milli>>(steady_clock::now() - start).count();
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
