#ifndef OPEN_FIGHT_GAME_CONTEXT_H
#define OPEN_FIGHT_GAME_CONTEXT_H

#include "fileReader.h"
#include "input.h"
#include "playerAgent.h"
#include "playerBridge.h"
#include "renderBackend.h"
#include "sprite.h"
#include <memory>
#include <string>
#include <vector>

struct GameSetup {
    std::string player_one_path;
    std::string player_two_path;
    std::string moves_path;
    std::string background_path;
};

struct FrameInput {
    bool players[2][KEY_MAX] = {{false}};
};

class GameContext {
private:
    PlayerAgent player_one;
    PlayerAgent player_two;
    Sprite background;
    PlayerBridge bridge;
    bool initialized;

    void updateObjects();

public:
    GameContext();
    ~GameContext();

    bool setup(const GameSetup &setup, RenderBackend &renderer,
               const ReadTextFileCallback &read_text_file = ReadTextFileCallback());
    void update(const FrameInput &input, float delta_ms);
    void render(RenderBackend &renderer);
    void shutdown();
};

#endif
