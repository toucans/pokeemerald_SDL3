#include <SDL3/SDL.h>
#include <stdlib.h>
#include <types.h>
//#include "src/map_loader.h"
#include "pokeemerald/maps.h"
#include "src/game.h"


#define GAME_WIDTH  240
#define GAME_HEIGHT 160
#define TARGET_FPS 60
#define FRAME_TIME_MS (1000 / TARGET_FPS)



int main(void) {

    GameState state = {0};
    SDL_Window *window;
    SDL_Event e;

    Uint64 ticks_last_frame = 0;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Emerald", GAME_WIDTH * 4, GAME_HEIGHT * 4, SDL_WINDOW_RESIZABLE, &window, &state.renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    } 
    SDL_SetWindowAspectRatio(window, 1.5, 1.5); //Set window to 240/160 = 1.5 aspect ratio

    // Enable VSync
    SDL_SetRenderVSync( state.renderer, 1 );

    // Logical size: keeps rendering at 240x160 regardless of window scale
    SDL_SetRenderLogicalPresentation(state.renderer, GAME_WIDTH, GAME_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    SDL_Init(SDL_INIT_VIDEO);

    game_init(&state);
    game_render(&state); // warmup: forces GPU initialization before the game loop

    int counter = 0;


    int running = 1;
    // Main game loop
    while (running) {
        state.input = 0;
        while (SDL_PollEvent(&e)) {
            //printf("Counter: %d\n", e.type);
            if (e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                running = 0;
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        running = 0;
                        break;
                    case SDL_SCANCODE_1:
                        SDL_SetWindowSize(window, GAME_WIDTH, GAME_HEIGHT);
                        break;
                    case SDL_SCANCODE_2:
                        SDL_SetWindowSize(window, GAME_WIDTH * 2, GAME_HEIGHT * 2);
                        break;
                    case SDL_SCANCODE_3:
                        SDL_SetWindowSize(window, GAME_WIDTH * 3, GAME_HEIGHT * 3);
                        break;
                    case SDL_SCANCODE_4:
                        SDL_SetWindowSize(window, GAME_WIDTH * 4, GAME_HEIGHT * 4);
                        break;
                    case SDL_SCANCODE_UP:    state.input |= INPUT_UP;    break;
                    case SDL_SCANCODE_DOWN:  state.input |= INPUT_DOWN;  break;
                    case SDL_SCANCODE_LEFT:  state.input |= INPUT_LEFT;  break;
                    case SDL_SCANCODE_RIGHT: state.input |= INPUT_RIGHT; break;
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
        if (counter % 60 == 0) {
            //do every second
        }
        
    }

    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

