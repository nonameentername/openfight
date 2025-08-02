#ifndef OPEN_FIGHT_MOVES_H
#define OPEN_FIGHT_MOVES_H

#include "input.h"
#include "keyState.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

class MoveSequence {
public:
    std::vector<KeyState *> keys;
    std::string trigger;

    MoveSequence(std::string keys, std::string trigger);
    ~MoveSequence();
};

class Moves {
private:
    std::vector<MoveSequence *> moves;
    void buildKey(std::list<KeyState> key_queue, std::map<std::string, int> &state, std::vector<KeyState> &result);

public:
    Moves();
    ~Moves();

    void initialize(std::string file_name);
    std::string getTrigger(std::list<KeyState> key_queue, std::map<std::string, int> &state);
};

#endif
