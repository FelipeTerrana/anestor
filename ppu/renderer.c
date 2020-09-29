#include "renderer.h"

#include <stdlib.h>
#include "screen.h"

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
        // TODO PPU rendering
    }

    *stopSignal = true;
    return 0;
}