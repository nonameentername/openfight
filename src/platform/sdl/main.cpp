#include "configuration.h"
#include "gameContext.h"
#include "global.h"
#include "graphics.h"
#include "graphicsCore.h"
#ifdef __EMSCRIPTEN__
#include "sdlRenderBackend.h"
#include <emscripten.h>
#else
#include "sdlGlRenderBackend.h"
#endif
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

struct MainLoopState {
    SDL_Window *window;
#ifdef __EMSCRIPTEN__
    SDL_Renderer *renderer;
    SdlRenderBackend *render_backend;
#else
    SDL_GLContext gl_context;
    SdlGlRenderBackend *render_backend;
#endif
    Input *input;
    GameContext *game;
    bool running;
    float game_time;
    float accumulator_ms;
    Uint32 last_time;
    int frames;
};

static void runFrame(void *arg) {
    MainLoopState *state = static_cast<MainLoopState *>(arg);
    if (!state->running || state->input->quitGame()) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    static const float fixed_step_ms = 10.0f;
    static const float max_frame_ms = 100.0f;
    static const int max_updates_per_frame = 5;

    float frame_time = getGameTime();
    float delta_ms = frame_time - state->game_time;
    if (delta_ms > max_frame_ms)
        delta_ms = max_frame_ms;
    state->game_time = frame_time;
    state->accumulator_ms += delta_ms;

    SDL_Event event = state->input->poll();

    if (event.type == SDL_QUIT) {
        state->running = false;
    } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        int sw = event.window.data1;
        int sh = event.window.data2;

        SDL_SetWindowSize(state->window, sw, sh);
        graphics->resizeWindow(sw, sh, *state->render_backend);
    }

    FrameInput frame_input;
    bool *keys = state->input->getKeys(0);
    for (int i = 0; i < KEY_MAX; i++)
        frame_input.players[0][i] = keys[i];

    keys = state->input->getKeys(1);
    for (int i = 0; i < KEY_MAX; i++)
        frame_input.players[1][i] = keys[i];

    int updates = 0;
    while (state->accumulator_ms >= fixed_step_ms && updates < max_updates_per_frame) {
        state->game->update(frame_input, fixed_step_ms);
        state->accumulator_ms -= fixed_step_ms;
        updates++;
    }
    if (updates == max_updates_per_frame)
        state->accumulator_ms = 0.0f;

    state->render_backend->beginFrame();

    state->game->render(*state->render_backend);

#ifdef __EMSCRIPTEN__
    SDL_RenderPresent(state->renderer);
#else
    SDL_GL_SwapWindow(state->window);
#endif

    state->frames++;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > state->last_time + 1000) {
        float fps = state->frames * 1000.0f / (currentTime - state->last_time);
        (void)fps;
        state->last_time = currentTime;
        state->frames = 0;
    }

}

#ifdef __cplusplus
extern "C"
#endif
    int
    main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    cout << "SDL Version: " << SDL_GetCompiledVersion() << endl;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }

#ifdef __EMSCRIPTEN__
    SDL_Window *window = SDL_CreateWindow("OpenFight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 400,
                                          SDL_WINDOW_RESIZABLE);
#else
    SDL_Window *window = SDL_CreateWindow("OpenFight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width,
                                          screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
#endif

    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

#ifdef __EMSCRIPTEN__
    SDL_Renderer *sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdl_renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SdlRenderBackend render_backend(sdl_renderer);
#else
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(0); // Turn off VSync because it was causing low FPS

    SdlGlRenderBackend render_backend;
#endif

    graphics->initialize(screen_width, screen_height, render_backend);
    cout << "VideoSystem Init OK" << endl;

    Configuration configuration("data/config.yml");
    configuration.read();

    Input input;
    input.addPlayer(configuration.getConfigKeys(true), configuration.getConfigDevice(true));
    input.addPlayer(configuration.getConfigKeys(false), configuration.getConfigDevice(false));

    GameSetup setup = {"data/ryu/ryu.yml", "data/ryu/ryu.yml", "data/ryu/moves.yml", "data/background.png"};

    GameContext game;
    game.setup(setup, render_backend);

    MainLoopState state = {};
    state.window = window;
#ifdef __EMSCRIPTEN__
    state.renderer = sdl_renderer;
#else
    state.gl_context = glContext;
#endif
    state.render_backend = &render_backend;
    state.input = &input;
    state.game = &game;
    state.running = true;
    state.game_time = getGameTime();
    state.accumulator_ms = 0.0f;
    state.last_time = SDL_GetTicks();
    state.frames = 0;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(runFrame, &state, 0, 1);
#else
    while (state.running && !input.quitGame())
        runFrame(&state);
#endif

    game.shutdown();
#ifdef __EMSCRIPTEN__
    SDL_DestroyRenderer(sdl_renderer);
#else
    SDL_GL_DeleteContext(glContext);
#endif
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
