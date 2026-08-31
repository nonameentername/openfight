#ifndef OPEN_FIGHT_PLAYER_AGENT_H
#define OPEN_FIGHT_PLAYER_AGENT_H

#include "fileReader.h"
#include "input.h"
#include "keyState.h"
#include "moves.h"
#include "player.h"
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

class PlayerAgent {
private:
    std::shared_ptr<Player> player;
    std::unique_ptr<Moves> moves;
    static const int QUEUE_KEY_MAX = 20;
    static const int INPUT_FRAME_MAX = 10;
    bool previous_keys[KEY_MAX];
    unsigned int current_frame;
    std::list<KeyState> key_queue;
    std::map<std::string, int> time_held;

public:
    PlayerAgent();
    virtual ~PlayerAgent() = default;

    std::shared_ptr<Player> getPlayer();
    bool initialize(std::string file_name, std::string moves_file, bool player_one,
                    const ReadTextFileCallback &read_text_file = ReadTextFileCallback());
    void update(bool *keys);
    void draw(RenderBackend &renderer);
};

#endif
