#include "configuration.h"
#include "gameContext.h"
#include "global.h"
#include "graphics.h"
#include "graphicsCore.h"
#include "sdlGlRenderBackend.h"
#include "sdlInput.h"
#include "utilities.h"
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

static const char *SDL_GetCompiledVersion() {
    static char version[32];
    SDL_version compiled;
    SDL_VERSION(&compiled);

    snprintf(version, sizeof(version), "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);
    return version;
}

#ifdef __cplusplus
extern "C"
#endif

    int
    main(int argc, char *argv[]) {
    cout << "SDL Version: " << SDL_GetCompiledVersion() << endl;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("OpenFight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width,
                                          screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(0); // Turn off VSync because it was causing low FPS

    SdlGlRenderBackend sdl_render_backend;
    graphics->initialize(screen_width, screen_height, sdl_render_backend);
    cout << "VideoSystem Init OK" << endl;

    Configuration configuration("data/config.yml");
    configuration.read();

    Input input;
    input.addPlayer(configuration.getConfigKeys(true), configuration.getConfigDevice(true));
    input.addPlayer(configuration.getConfigKeys(false), configuration.getConfigDevice(false));

    GameSetup setup = {"data/ryu/ryu.yml", "data/ryu/ryu.yml", "data/ryu/moves.yml", "data/background.png"};

    GameContext game;
    game.setup(setup, sdl_render_backend);

    Uint32 lastTime = SDL_GetTicks();
    int frames = 0;

    bool running = true;
    float game_time = getGameTime();

    while (running && !input.quitGame()) {

        if (updateGame(game_time)) {
            float frame_time = getGameTime();
            float delta_ms = frame_time - game_time;

            SDL_Event event = input.poll();

            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                int sw = event.window.data1;
                int sh = event.window.data2;

                SDL_SetWindowSize(window, sw, sh);
                graphics->resizeWindow(sw, sh, sdl_render_backend);
            }

            FrameInput frame_input;
            bool *keys = input.getKeys(0);
            for (int i = 0; i < KEY_MAX; i++)
                frame_input.players[0][i] = keys[i];

            keys = input.getKeys(1);
            for (int i = 0; i < KEY_MAX; i++)
                frame_input.players[1][i] = keys[i];

            game.update(frame_input, delta_ms);

            sdl_render_backend.beginFrame();

            game.render(sdl_render_backend);

            SDL_GL_SwapWindow(window);

            frames++;
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime > lastTime + 1000) {
                float fps = frames * 1000.0f / (currentTime - lastTime);
                // printf("FPS: %.2f\n", fps);
                lastTime = currentTime;
                frames = 0;
            }

            game_time = frame_time;
        }
    }

    game.shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
