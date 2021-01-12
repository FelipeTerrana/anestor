#include "renderer.h"

#include <stdlib.h>
#include "screen.h"
#include "../clock_waiter.h"

#define TOTAL_SCANLINES 262
#define VBLANK_SCANLINES 20
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

    ClockWaiter* clockWaiter = clockWaiterInit(PPU_CLOCK_SPEED_HZ);

    while ( !screenIsClosed(renderer->screen) )
    {
        clockWaiterStart(clockWaiter);

        ppuRegistersStopVblank(renderer->memory);
        ppuMemoryRender(renderer->memory, renderer->screen);

        clockWaiterFinish(clockWaiter, (TOTAL_SCANLINES - VBLANK_SCANLINES) * CYCLES_PER_SCANLINE);

        clockWaiterStart(clockWaiter);
        ppuRegistersStartVblank(renderer->memory);
        clockWaiterFinish(clockWaiter, VBLANK_SCANLINES * CYCLES_PER_SCANLINE);
    }

    clockWaiterShutdown(clockWaiter);
    *stopSignal = true;
    return 0;
}