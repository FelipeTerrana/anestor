#include "screen.h"

#include <SDL.h>
#include <stdlib.h>
#include "../flag_ops.h"

struct screen__ {
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    uint8_t xScroll, yScroll;
    uint8_t ppumask;

    NesPixel pixels[2 * NATIVE_HEIGHT][2 * NATIVE_WIDTH];
};

typedef struct {
    uint8_t r, g, b;
} RgbPixel;



static const RgbPixel HUE_TO_RGB[] = {
        [0x0] = {0xCC, 0xCC, 0xCC},
        [0x1] = {0x00, 0x00, 0xFF},
        [0x2] = {0x80, 0x00, 0xFF},
        [0x3] = {0xFF, 0x00, 0xFF},
        [0x4] = {0xFF, 0x00, 0x80},
        [0x5] = {0xFF, 0x00, 0x00},
        [0x6] = {0xFF, 0x80, 0x00},
        [0x7] = {0xFF, 0xFF, 0x00},
        [0x8] = {0x80, 0xFF, 0x00},
        [0x9] = {0x00, 0xFF, 0x00},
        [0xA] = {0x00, 0xFF, 0x80},
        [0xB] = {0x00, 0xFF, 0xFF},
        [0xC] = {0x00, 0x80, 0xFF},
        [0xD] = {0x44, 0x44, 0x44},
        [0xE] = {0x00, 0x00, 0x00},
        [0xF] = {0x00, 0x00, 0x00}
};



static RgbPixel nesPixelToRgb(Screen* screen, NesPixel nes)
{
    // TODO apply brightness
    // TODO apply mask
    uint8_t hue = getFlagValue(nes, 0xF);

    return HUE_TO_RGB[hue];
}



static void renderRgbPixel(Screen* screen, int x, int y, const RgbPixel* pixel)
{
    SDL_Rect pixelLarge;
    pixelLarge.w = pixelLarge.h = RESOLUTION_MULTIPLIER;

    SDL_SetRenderDrawColor(screen->sdlRenderer, pixel->r, pixel->g, pixel->b, SDL_ALPHA_OPAQUE);

    pixelLarge.x = (x - screen->xScroll) * RESOLUTION_MULTIPLIER;
    pixelLarge.y = (y - screen->yScroll) * RESOLUTION_MULTIPLIER;
    SDL_RenderFillRect(screen->sdlRenderer, &pixelLarge);
}



Screen* screenInit()
{
    Screen* screen = malloc( sizeof(struct screen__) );

    screen->sdlWindow = SDL_CreateWindow("aNEStor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           NATIVE_WIDTH * RESOLUTION_MULTIPLIER, NATIVE_HEIGHT * RESOLUTION_MULTIPLIER,
                                           SDL_WINDOW_SHOWN);

    screen->sdlRenderer = SDL_CreateRenderer(screen->sdlWindow, -1,
                                               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    screenSetScroll(screen, 0, 0);
    NesPixel black = 0x0F;
    int x, y;
    for(y = 0; y < 2 * NATIVE_HEIGHT; y++)
        for(x = 0; x < 2 * NATIVE_WIDTH; x++)
            screenSetPixel(screen, x, y, black);

    screenRefresh(screen);

    return screen;
}



void screenShutdown(Screen* screen)
{
    SDL_DestroyRenderer(screen->sdlRenderer);
    SDL_DestroyWindow(screen->sdlWindow);

    free(screen);
}



void screenSetPixel(Screen* screen, int x, int y, NesPixel pixel)
{
    screen->pixels[y][x] = pixel;
}



void screenSetScroll(Screen* screen, uint8_t xScroll, uint8_t yScroll)
{
    screen->xScroll = xScroll;
    screen->yScroll = yScroll;
}



void screenSetPpumask(Screen* screen, uint8_t ppumask)
{
    screen->ppumask = ppumask;
}



void screenRefresh(Screen* screen)
{
    int x, y;

    for(y = screen->yScroll; y < screen->yScroll + NATIVE_HEIGHT; y++)
    {
        for(x = screen->xScroll; x < screen->xScroll + NATIVE_WIDTH; x++)
        {
            RgbPixel rgb = nesPixelToRgb(screen, screen->pixels[y][x]);
            renderRgbPixel(screen, x, y, &rgb);
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