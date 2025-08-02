#include "moves.h"
#include "utilities.h"

using namespace std;

MoveSequence::MoveSequence(string keys, string trigger) {
    vector<string> move_tokens = splitString(keys, ", ");

    for (int i = 0; i < move_tokens.size(); i++) {
        int time_held = 0;
        bool released = move_tokens[i].find("~", 0) == 0;
        bool held = move_tokens[i].find("/", 0) == 0;
        string key_name;

        if (released || held)
            key_name = move_tokens[i].substr(1);
        else
            key_name = move_tokens[i];

        int n_pos = key_name.find(":", 0);

        if (n_pos > 0) {
            time_held = atoi(key_name.substr(n_pos + 1).c_str());
            key_name.erase(n_pos);
        }

        for (int j = 0; j < KEY_MAX; j++) {
            if (key_name == Input::getKeyName(j)) {
                this->keys.push_back(new KeyState(j, time_held, !released, held));
                break;
            }
        }
    }

    this->trigger = trigger;
}

MoveSequence::~MoveSequence() {
    for (int i = 0; i < keys.size(); i++)
        delete keys[i];
}

Moves::Moves() {
}

Moves::~Moves() {
    for (int i = 0; i < moves.size(); i++)
        delete moves[i];
}

void Moves::buildKey(list<KeyState> key_queue, map<string, int> &state, vector<KeyState> &result) {
    list<KeyState>::iterator it;

    for (it = key_queue.begin(); it != key_queue.end(); it++) {
        string key_name = Input::getKeyName(it->getKey());
        it->setTimeHeld(state[key_name]);

        result.push_back(*it);
    }
}

string Moves::getTrigger(list<KeyState> key_queue, map<string, int> &state) {
    string result = "NONE";
    vector<KeyState> keys;

    for (int i = KEY_UP; i <= KEY_RIGHT; i++) {
        string key_name = Input::getKeyName(i);

        if (state[key_name] > 1) {
            KeyState k(i, true);
            k.setTimeHeld(state[key_name]);
            k.setHeld(true);
            keys.push_back(k);
        }
    }

    if (keys.size() > 0) {
        for (int i = 0; i < moves.size(); i++)
            if (keys.back() == moves[i]->keys.back() &&
                search(keys.begin(), keys.end(), moves[i]->keys.begin(), moves[i]->keys.end()) != keys.end())
                result = moves[i]->trigger;
    }
    //
    for (int i = KEY_A; i <= KEY_Z; i++) {
        string key_name = Input::getKeyName(i);

        if (state[key_name] > 1) {
            KeyState k(i, true);
            k.setTimeHeld(state[key_name]);
            k.setHeld(true);
            keys.push_back(k);
        }
    }

    if (keys.size() > 0) {
        for (int i = 0; i < moves.size(); i++)
            if (keys.back() == moves[i]->keys.back() &&
                search(keys.begin(), keys.end(), moves[i]->keys.begin(), moves[i]->keys.end()) != keys.end())
                result = moves[i]->trigger;
    }
    //
    if (key_queue.size() > 0)
        buildKey(key_queue, state, keys);

    if (keys.size() > 0) {
        for (int i = 0; i < moves.size(); i++)
            if (keys.back() == moves[i]->keys.back() &&
                search(keys.begin(), keys.end(), moves[i]->keys.begin(), moves[i]->keys.end()) != keys.end())
                return moves[i]->trigger;
    }

    return result;
}
