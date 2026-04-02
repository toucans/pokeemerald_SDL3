#include <SDL3/SDL.h>
#include <stdlib.h>
#include <types.h>
#include "pokeemerald/maps.h"
#include "src/game.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define GAME_WIDTH  240
#define GAME_HEIGHT 160
#define TARGET_FPS 60

static GameState state;
static SDL_Window *window;
static SDL_Event e;
static int running = 1;
static Uint64 ticks_last_frame;
static int counter = 0;

static void main_loop(void) {
    state.input = 0;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            running = 0;
        } else if (e.type == SDL_EVENT_KEY_DOWN) {
            switch (e.key.scancode) {
                case SDL_SCANCODE_ESCAPE:       running = 0;                    break;
                case SDL_SCANCODE_1: SDL_SetWindowSize(window, GAME_WIDTH,     GAME_HEIGHT);     break;
                case SDL_SCANCODE_2: SDL_SetWindowSize(window, GAME_WIDTH * 2, GAME_HEIGHT * 2); break;
                case SDL_SCANCODE_3: SDL_SetWindowSize(window, GAME_WIDTH * 3, GAME_HEIGHT * 3); break;
                case SDL_SCANCODE_4: SDL_SetWindowSize(window, GAME_WIDTH * 4, GAME_HEIGHT * 4); break;
                case SDL_SCANCODE_UP:     state.input |= INPUT_UP;    break;
                case SDL_SCANCODE_DOWN:   state.input |= INPUT_DOWN;  break;
                case SDL_SCANCODE_LEFT:   state.input |= INPUT_LEFT;  break;
                case SDL_SCANCODE_RIGHT:  state.input |= INPUT_RIGHT; break;
                case SDL_SCANCODE_RETURN: state.input |= INPUT_ENTER; break;
                default: break;
            }
        }
    }

    uint64_t ticks_now = SDL_GetPerformanceCounter();
    state.timestep = (double)(ticks_now - ticks_last_frame) / SDL_GetPerformanceFrequency();
    if (state.timestep > 1.0 / TARGET_FPS) state.timestep = 1.0 / TARGET_FPS;
    ticks_last_frame = ticks_now;

    game_update(&state);
    game_render(&state);

    counter++;

#ifdef __EMSCRIPTEN__
    if (!running) emscripten_cancel_main_loop();
#endif
}

int main(void) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Emerald", GAME_WIDTH * 4, GAME_HEIGHT * 4, SDL_WINDOW_RESIZABLE, &window, &state.renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    SDL_SetWindowAspectRatio(window, 1.5, 1.5);
    SDL_SetRenderVSync(state.renderer, 1);
    SDL_SetRenderLogicalPresentation(state.renderer, GAME_WIDTH, GAME_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    game_init(&state);
    game_render(&state);

    ticks_last_frame = SDL_GetPerformanceCounter() - SDL_GetPerformanceFrequency() / TARGET_FPS;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (running) main_loop();

    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
#endif
    return 0;
}

