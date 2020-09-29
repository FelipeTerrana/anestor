#include "screen.h"

#include <SDL.h>
#include <stdlib.h>

struct screen__ {
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    uint8_t xScroll, yScroll;

    Pixel pixels[2 * NATIVE_HEIGHT][2 * NATIVE_WIDTH];
};



Screen* screenInit()
{
    Screen* screen = malloc( sizeof(struct screen__) );

    screen->sdlWindow = SDL_CreateWindow("aNEStor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           NATIVE_WIDTH * RESOLUTION_MULTIPLIER, NATIVE_HEIGHT * RESOLUTION_MULTIPLIER,
                                           SDL_WINDOW_SHOWN);

    screen->sdlRenderer = SDL_CreateRenderer(screen->sdlWindow, -1,
                                               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    screenSetScroll(screen, 0, 0);
    Pixel black = {0x00, 0x00, 0x00};
    int x, y;
    for(y = 0; y < 2 * NATIVE_HEIGHT; y++)
        for(x = 0; x < 2 * NATIVE_WIDTH; x++)
            screenSetPixel(screen, x, y, &black);

    screenRefresh(screen);

    return screen;
}



void screenShutdown(Screen* screen)
{
    SDL_DestroyRenderer(screen->sdlRenderer);
    SDL_DestroyWindow(screen->sdlWindow);

    free(screen);
}



void screenSetPixel(Screen* screen, int x, int y, const Pixel* pixel)
{
    screen->pixels[y][x] = *pixel;
}



void screenSetScroll(Screen* screen, uint8_t xScroll, uint8_t yScroll)
{
    screen->xScroll = xScroll;
    screen->yScroll = yScroll;
}



void screenRefresh(Screen* screen)
{
    int x, y;
    SDL_Rect pixelLarge;
    pixelLarge.w = pixelLarge.h = RESOLUTION_MULTIPLIER;

    for(y = screen->yScroll; y < screen->yScroll + NATIVE_HEIGHT; y++)
    {
        for(x = screen->xScroll; x < screen->xScroll + NATIVE_WIDTH; x++)
        {
            SDL_SetRenderDrawColor(screen->sdlRenderer, screen->pixels[y][x].r,
                                                        screen->pixels[y][x].g,
                                                        screen->pixels[y][x].b,
                                                        SDL_ALPHA_OPAQUE);

            pixelLarge.x = (x - screen->xScroll) * RESOLUTION_MULTIPLIER;
            pixelLarge.y = (y - screen->yScroll) * RESOLUTION_MULTIPLIER;
            SDL_RenderFillRect(screen->sdlRenderer, &pixelLarge);
        }
    }

    SDL_RenderPresent(screen->sdlRenderer);
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