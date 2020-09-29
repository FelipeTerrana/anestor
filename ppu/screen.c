#include "screen.h"

#include <SDL.h>
#include <stdlib.h>

struct screen__ {
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
};



Screen* screenInit()
{
    Screen* screen = malloc( sizeof(struct screen__) );

    screen->sdlWindow = SDL_CreateWindow("aNEStor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           NATIVE_WIDTH * RESOLUTION_MULTIPLIER, NATIVE_HEIGHT * RESOLUTION_MULTIPLIER,
                                           SDL_WINDOW_SHOWN);

    screen->sdlRenderer = SDL_CreateRenderer(screen->sdlWindow, -1,
                                               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderPresent(screen->sdlRenderer);

    return screen;
}



void screenShutdown(Screen* screen)
{
    SDL_DestroyRenderer(screen->sdlRenderer);
    SDL_DestroyWindow(screen->sdlWindow);

    free(screen);
}



bool screenIsClosed(Screen* screen)
{
    SDL_Event event;
    while ( SDL_PollEvent(&event) )
    {
        if(event.type == SDL_QUIT)
            return true;
    }

    return false;
}