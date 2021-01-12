#include "screen.h"

#include <SDL.h>
#include <stdlib.h>
#include "../flag_ops.h"

// PPUMASK
#define GREYSCALE_MASK 0x01
#define EMPHASIZE_RED_MASK 0x20
#define EMPHASIZE_GREEN_MASK 0x40
#define EMPHASIZE_BLUE_MASK 0x80

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))

struct screen__ {
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* sdlTexture;
    int xScroll, yScroll;
    uint8_t ppumask;

    NesPixel nesPixels[2 * NATIVE_HEIGHT][2 * NATIVE_WIDTH];
};

typedef struct {
    uint8_t r, g, b;
} RgbPixel;



static const RgbPixel NES_PIXEL_TO_RGB[] = {
        {84, 84, 84},
        {0, 30, 116},
        {8, 16, 144},
        {48, 0, 136},
        {68, 0, 100},
        {92, 0,  48},
        {84, 4, 0},
        {60, 24, 0},
        {32, 42, 0},
        {8, 58, 0},
        {0, 64, 0},
        {0, 60, 0},
        {0, 50, 60},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},

        {152, 150, 152},
        {8, 76, 196},
        {48, 50, 236},
        {92, 30, 228},
        {136, 20, 176},
        {160, 20, 100},
        {152, 34, 32},
        {120, 60, 0},
        {84, 90, 0},
        {40, 114, 0},
        {8, 124, 0},
        {0, 118, 40},
        {0, 102, 120},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},

        {236, 238, 236},
        {76, 154, 236},
        {120, 124, 236},
        {176, 98, 236},
        {228, 84, 236},
        {236, 88, 180},
        {236, 106, 100},
        {212, 136, 32},
        {160, 170, 0},
        {116, 196, 0},
        {76, 208, 32},
        {56, 204, 108},
        {56, 180, 204},
        {60, 60, 60},
        {0, 0, 0},
        {0, 0, 0},

        {236, 238, 236},
        {168, 204, 236},
        {188, 188, 236},
        {212, 178, 236},
        {236, 174, 236},
        {236, 174, 212},
        {236, 180, 176},
        {228, 196, 144},
        {204, 210, 120},
        {180, 222, 120},
        {168, 226, 144},
        {152, 226, 180},
        {160, 214, 228},
        {160, 162, 160},
        {0, 0, 0},
        {0, 0, 0}
};



static RgbPixel nesPixelToRgb(Screen* screen, const NesPixel* nes)
{
    RgbPixel rgbPixel = NES_PIXEL_TO_RGB[nes->value];
    
    if( getFlagValue(screen->ppumask, GREYSCALE_MASK) == 1 )
        rgbPixel.r = rgbPixel.g = rgbPixel.b = (0.3 * rgbPixel.r + 0.59 * rgbPixel.g + 0.11 * rgbPixel.b) / 3;

    if( getFlagValue(screen->ppumask, EMPHASIZE_RED_MASK) == 1 )
    {
        rgbPixel.r = min(255, rgbPixel.r * 1.25);
        rgbPixel.g *= 0.75;
        rgbPixel.b *= 0.75;
    }

    if( getFlagValue(screen->ppumask, EMPHASIZE_GREEN_MASK) == 1 )
    {
        rgbPixel.r *= 0.75;
        rgbPixel.g = min(255, rgbPixel.g * 1.25);
        rgbPixel.b *= 0.75;
    }

    if( getFlagValue(screen->ppumask, EMPHASIZE_BLUE_MASK) == 1 )
    {
        rgbPixel.r *= 0.75;
        rgbPixel.g *= 0.75;
        rgbPixel.b = min(255, rgbPixel.b * 1.25);
    }
    
    return rgbPixel;
}



static bool replacePixel(const NesPixel* new, const NesPixel* old)
{
    return (
            old->type == BACKGROUND ||

            (new->type == SPRITE && old->type == SPRITE &&
             !new->isTransparent && new->priority < old->priority) ||

            (new->type == BACKGROUND && old->type == SPRITE &&
             !new->isTransparent && (old->isTransparent || old->isBehindBackground))
            );
}



Screen* screenInit()
{
    Screen* screen = malloc( sizeof(struct screen__) );

    screen->sdlWindow = SDL_CreateWindow("aNEStor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           NATIVE_WIDTH * RESOLUTION_MULTIPLIER, NATIVE_HEIGHT * RESOLUTION_MULTIPLIER,
                                           SDL_WINDOW_SHOWN);

    screen->sdlRenderer = SDL_CreateRenderer(screen->sdlWindow, -1,
                                               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    screen->sdlTexture = SDL_CreateTexture(screen->sdlRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING,
                                           NATIVE_WIDTH * RESOLUTION_MULTIPLIER, NATIVE_HEIGHT * RESOLUTION_MULTIPLIER);

    screenSetScroll(screen, 0, 0);
    NesPixel black = {0x0F, BACKGROUND, 0, true};
    int x, y;
    for(y = 0; y < 2 * NATIVE_HEIGHT; y++)
        for(x = 0; x < 2 * NATIVE_WIDTH; x++)
            screenSetPixel(screen, x, y, &black);

    screenRefresh(screen);

    return screen;
}



void screenShutdown(Screen* screen)
{
    SDL_DestroyTexture(screen->sdlTexture);
    SDL_DestroyRenderer(screen->sdlRenderer);
    SDL_DestroyWindow(screen->sdlWindow);

    free(screen);
}



/**
 * ORDER MATTERS! Render sprites before background each frame
 */
void screenSetPixel(Screen* screen, int x, int y, const NesPixel* pixel)
{
    if( replacePixel(pixel, &screen->nesPixels[y][x]) )
        screen->nesPixels[y][x] = *pixel;
}



void screenSetScroll(Screen* screen, int xScroll, int yScroll)
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
    // TODO check if rendering can be further optimized
    int x, y;
    int offsetX, offsetY;

    uint32_t* textureBuffer;
    int pitch;

    SDL_LockTexture(screen->sdlTexture, NULL, (void**) &textureBuffer, &pitch);

    for(y = 0; y < 2 * NATIVE_HEIGHT; y++)
    {
        for(x = 0; x < 2 * NATIVE_WIDTH; x++)
        {
            if(y >= screen->yScroll && y < screen->yScroll + NATIVE_HEIGHT &&
               x >= screen->xScroll && x < screen->xScroll + NATIVE_WIDTH)
            {
                RgbPixel rgb = nesPixelToRgb(screen, &screen->nesPixels[y][x]);

                for(offsetY = 0; offsetY < RESOLUTION_MULTIPLIER; offsetY++)
                {
                    for(offsetX = 0; offsetX < RESOLUTION_MULTIPLIER; offsetX++)
                    {
                        int bufferIndex = (RESOLUTION_MULTIPLIER * (y - screen->yScroll) + offsetY) * (NATIVE_WIDTH * RESOLUTION_MULTIPLIER)
                                          + (RESOLUTION_MULTIPLIER * (x - screen->xScroll) + offsetX);

                        textureBuffer[bufferIndex] = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
                    }
                }
            }

            screen->nesPixels[y][x].type = BACKGROUND; // Prepares pixel for next frame rendering
        }
    }

    SDL_UnlockTexture(screen->sdlTexture);
    SDL_RenderCopy(screen->sdlRenderer, screen->sdlTexture, NULL, NULL);
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