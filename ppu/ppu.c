#include "ppu.h"

#include <stdlib.h>
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



PpuMemory* ppuGetMemory(Ppu* ppu)
{
    return ppu->memory;
}