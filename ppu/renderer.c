#include "renderer.h"

#include <stdlib.h>

struct ppu_renderer__ {
    PpuMemory* memory;
};



PpuRenderer* ppuRendererInit(PpuMemory* memory)
{
    PpuRenderer* renderer = malloc( sizeof(struct ppu_renderer__) );
    renderer->memory = memory;
    return renderer;
}



void ppuRendererShutdown(PpuRenderer* renderer)
{
    free(renderer);
}