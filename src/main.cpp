#include <SDL2/SDL.h>
#include "utilities.h"
#include "input.h"
#include "animation.h"
#include "playerAgent.h"
#include "sprite.h"
#include "graphicsCore.h"
#include "graphics.h"
#include "configuration.h"
#include "playerBridge.h"
#include "global.h"

using namespace std;

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0) {
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("OpenFight",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screen_width, screen_height,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        SDL_Quit();
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1); // vsync

    graphics->initialize(screen_width, screen_height);

    Configuration configuration("data/config.xml");
    configuration.read();

    Input input;
    input.addPlayer(configuration.getConfigKeys(true), configuration.getConfigDevice(true));
    input.addPlayer(configuration.getConfigKeys(false), configuration.getConfigDevice(false));

    PlayerAgent player;
    player.initialize("data/ryu/ryu.xml", "data/ryu/moves.xml", true);

    PlayerAgent player2;
    player2.initialize("data/ryu/ryu.xml", "data/ryu/moves.xml", false);

    player.getPlayer()->setOpponent(player2.getPlayer());
    player2.getPlayer()->setOpponent(player.getPlayer());

    Sprite background;
    GLuint texture = texture_manager->addTexture("data/background.png", false);
    background.setTexture(texture, 200, 100);

    PlayerBridge bridge;
    bridge.initialize(player.getPlayer(), player2.getPlayer());

    object_manager->add("0", player.getPlayer());
    object_manager->add("1", player2.getPlayer());

    Uint32 lastTime = SDL_GetTicks();
    int frames = 0;

    bool running = true;

    while (running && !input.quitGame())
    {
        float game_time;

        if (updateGame(game_time))
        {
            SDL_Event event = input.poll();

            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_WINDOWEVENT &&
                        event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                int sw = event.window.data1;
                int sh = event.window.data2;

                SDL_SetWindowSize(window, sw, sh);
                graphics->resizeWindow(sw, sh);
            }

            bool *keys = input.getKeys(0);
            player.update(keys);

            bool *keys2 = input.getKeys(1);
            player2.update(keys2);

            string key = object_manager->first();
            while (key != "")
            {
                bool done = false;

                if (key != "0" && key != "1")
                    done = object_manager->get(key)->update();

                if (done) {
                    object_manager->remove(key);
                    break;
                }

                key = object_manager->next();
            }

            // Render
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();
            glTranslatef(-50.0f, -50.0f, -120.0f);

            bridge.update();
            background.draw(50, 50, 1, false);

            list<Player *> objects = object_manager->getSortedList();
            for (auto i = objects.begin(); i != objects.end(); ++i)
                (*i)->draw();

            SDL_GL_SwapWindow(window);

            frames++;
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime > lastTime + 1000) {
                float fps = frames * 1000.0f / (currentTime - lastTime);
                printf("FPS: %.2f\n", fps);
                lastTime = currentTime;
                frames = 0;
            }

            game_time = getGameTime();
        }
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
