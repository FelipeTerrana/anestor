#include "ppu.h"

#include <stdlib.h>

struct ppu__ {
    PpuMemory* memory;
};



Ppu* ppuInit(Cartridge* cartridge)
{
    Ppu* ppu = malloc( sizeof(struct ppu__) );
    ppu->memory = ppuMemoryInit(cartridge);
    return ppu;
}



void ppuShutdown(Ppu* ppu)
{
    ppuMemoryShutdown(ppu->memory);
    free(ppu);
}



PpuMemory* ppuGetMemory(Ppu* ppu)
{
    return ppu->memory;
}