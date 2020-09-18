#include "ppu.h"

#include <stdlib.h>
#include <stdbool.h>
#include "renderer.h"

struct ppu__ {
    PpuMemory* memory;
    PpuRenderer* renderer;
};



Ppu* ppuInit(Cartridge* cartridge)
{
    Ppu* ppu = malloc( sizeof(struct ppu__) );
    ppu->memory = ppuMemoryInit(cartridge);
    ppu->renderer = ppuRendererInit(ppu->memory);

    return ppu;
}



void ppuShutdown(Ppu* ppu)
{
    ppuRendererShutdown(ppu->renderer);
    ppuMemoryShutdown(ppu->memory);
    free(ppu);
}



int ppuLoop(void* data)
{
    void** inputArray = data;
    Ppu* ppu = inputArray[0];
    bool* stopSignal = inputArray[1];

    void* rendererInputArray[2] = {ppu->renderer, stopSignal};
    return ppuRendererLoop(rendererInputArray);
}



PpuMemory* ppuGetMemory(Ppu* ppu)
{
    return ppu->memory;
}