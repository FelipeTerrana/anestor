#include "renderer.h"

#include <stdlib.h>
#include <time.h>
#include "screen.h"
#include "../clock_rates.h"

#define SCANLINES 262
#define CYCLES_PER_SCANLINE 341

struct ppu_renderer__ {
    PpuMemory* memory;
    Screen* screen;
};



PpuRenderer* ppuRendererInit(PpuMemory* memory)
{
    PpuRenderer* renderer = malloc( sizeof(struct ppu_renderer__) );
    renderer->memory = memory;
    renderer->screen = screenInit();

    return renderer;
}



void ppuRendererShutdown(PpuRenderer* renderer)
{
    screenShutdown(renderer->screen);
    free(renderer);
}



int ppuRendererLoop(void* data)
{
    void** inputArray = data;
    PpuRenderer* renderer = inputArray[0];
    bool* stopSignal = inputArray[1];

    while ( !screenIsClosed(renderer->screen) )
    {
        clock_t realClockTicksToWait, realClockOnStart = clock();
        realClockTicksToWait = (CLOCKS_PER_SEC / PPU_CLOCK_SPEED_HZ) * (SCANLINES * CYCLES_PER_SCANLINE);

        ppuMemoryRender(renderer->memory, renderer->screen);

        while ((clock() - realClockOnStart) < realClockTicksToWait);
    }

    *stopSignal = true;
    return 0;
}