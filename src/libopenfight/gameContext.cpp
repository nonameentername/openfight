#include "gameContext.h"
#include "graphicsCore.h"
#include "utilities.h"
#include <list>

using namespace std;

GameContext::GameContext() : initialized(false) {
}

GameContext::~GameContext() {
    shutdown();
}

bool GameContext::setup(const GameSetup &setup, RenderBackend &renderer, const ReadTextFileCallback &read_text_file) {
    if (initialized)
        shutdown();

    texture_manager->setRenderBackend(&renderer);

    player_one.initialize(setup.player_one_path, setup.moves_path, true, read_text_file);
    player_two.initialize(setup.player_two_path, setup.moves_path, false, read_text_file);

    auto p2 = player_two.getPlayer().get();
    auto p1 = player_one.getPlayer().get();
    player_one.getPlayer()->setOpponent(p2);
    player_two.getPlayer()->setOpponent(p1);

    unsigned int texture = texture_manager->addTexture(setup.background_path, false);
    background.setTexture(texture, 200, 100);

    bridge.initialize(p1, p2);

    object_manager->add("0", p1);
    object_manager->add("1", p2);

    initialized = true;
    return true;
}

void GameContext::update(const FrameInput &input, float delta_ms) {
    (void)delta_ms;
    if (!initialized)
        return;

    bool player_one_keys[KEY_MAX];
    bool player_two_keys[KEY_MAX];

    for (int i = 0; i < KEY_MAX; i++) {
        player_one_keys[i] = input.players[0][i];
        player_two_keys[i] = input.players[1][i];
    }

    player_one.update(player_one_keys);
    player_two.update(player_two_keys);
    updateObjects();
}

void GameContext::updateObjects() {
    string key = object_manager->first();
    while (key != "") {
        bool done = false;

        if (key != "0" && key != "1")
            done = object_manager->get(key)->update();

        if (done) {
            object_manager->remove(key);
            break;
        }

        key = object_manager->next();
    }
}

void GameContext::render(RenderBackend &renderer) {
    if (!initialized)
        return;

    bridge.update();
    background.draw(renderer, 50, 50, 1, false);

    list<Player *> objects = object_manager->getSortedList();
    for (auto i = objects.begin(); i != objects.end(); ++i)
        (*i)->draw(renderer);
}

void GameContext::shutdown() {
    if (!initialized)
        return;

    object_manager->detach("0");
    object_manager->detach("1");
    object_manager->clear();
    texture_manager->clear();
    initialized = false;
}
