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
    
    
    int counter = 0;

    int running = 1;
    // Main game loop
    while (running) {
        while (SDL_PollEvent(&e)) {
            //printf("Counter: %d\n", e.type);
            if (e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                running = 0;
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.which) {
                    case SDLK_ESCAPE:
                        running = 0;
                        break;
                    case SDLK_1:
                        SDL_SetWindowSize(window, GAME_WIDTH, GAME_HEIGHT);
                        break;
                    case SDLK_2:
                        SDL_SetWindowSize(window, GAME_WIDTH * 2, GAME_HEIGHT * 2);
                        break;
                    case SDLK_3:
                        SDL_SetWindowSize(window, GAME_WIDTH * 3, GAME_HEIGHT * 3);
                        break;
                    case SDLK_4:
                        SDL_SetWindowSize(window, GAME_WIDTH * 4, GAME_HEIGHT * 4);
                        break;
                }
            }
        }
        uint64_t ticks_now = SDL_GetPerformanceCounter();
        state.timestep = (double)(ticks_now - ticks_last_frame) / SDL_GetPerformanceFrequency(); //time elapsed since the last frame in seconds
        ticks_last_frame = ticks_now;

        game_update(&state);
        game_render(&state);

        counter++;
        if (counter % 60 == 0) {
            //printf("Counter: %d\n", counter);
            printf("%u\n", state.player.x);
            printf("%u\n", state.player.y);
        }
        
    }

    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

