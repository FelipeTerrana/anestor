#ifndef ANESTOR_PPU_H
#define ANESTOR_PPU_H

#include "../cartridge/cartridge.h"
#include "memory.h"

typedef struct ppu__ Ppu;

Ppu* ppuInit(Cartridge* cartridge);
void ppuShutdown(Ppu* ppu);

PpuMemory* ppuGetMemory(Ppu* ppu);

#endif //ANESTOR_PPU_H
