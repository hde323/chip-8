#include "graphics.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int init_graphics() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("CHIP-8 Emulator",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              64 * WINDOW_SCALE,
                              32 * WINDOW_SCALE,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

void draw_graphics(Cpu *cpu) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (cpu->FrameBuffer[y * 64 + x]) {
                SDL_Rect pixel = { x * WINDOW_SCALE, y * WINDOW_SCALE,
                                   WINDOW_SCALE, WINDOW_SCALE };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void close_graphics() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
