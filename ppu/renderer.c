#include "renderer.h"

#include <stdlib.h>
#include <SDL.h>

#define NATIVE_WIDTH 256
#define NATIVE_HEIGHT 240
#define RESOLUTION_MULTIPLIER 2

struct ppu_renderer__ {
    PpuMemory* memory;
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
};



PpuRenderer* ppuRendererInit(PpuMemory* memory)
{
    PpuRenderer* renderer = malloc( sizeof(struct ppu_renderer__) );
    renderer->memory = memory;

    renderer->sdlWindow = SDL_CreateWindow("aNEStor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           NATIVE_WIDTH * RESOLUTION_MULTIPLIER, NATIVE_HEIGHT * RESOLUTION_MULTIPLIER,
                                           SDL_WINDOW_SHOWN);

    renderer->sdlRenderer = SDL_CreateRenderer(renderer->sdlWindow, -1,
                                               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderPresent(renderer->sdlRenderer);

    return renderer;
}



void ppuRendererShutdown(PpuRenderer* renderer)
{
    SDL_DestroyRenderer(renderer->sdlRenderer);
    SDL_DestroyWindow(renderer->sdlWindow);

    free(renderer);
}



int ppuRendererLoop(void* data)
{
    void** inputArray = data;
    PpuRenderer* renderer = inputArray[0];
    bool* stopSignal = inputArray[1];

    SDL_Event event;

    while ( !(*stopSignal) )
    {
        // TODO PPU rendering

        while ( SDL_PollEvent(&event) )
        {
            if(event.type == SDL_QUIT)
            {
                *stopSignal = true;
                break;
            }
        }
    }

    return 0;
}