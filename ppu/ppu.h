#ifndef ANESTOR_PPU_H
#define ANESTOR_PPU_H

#include "../cartridge/cartridge.h"
#include "memory/memory.h"

typedef struct ppu__ Ppu;

Ppu* ppuInit(Cartridge* cartridge);
void ppuShutdown(Ppu* ppu);

int ppuLoop(void* data);

PpuMemory* ppuGetMemory(Ppu* ppu);

#endif //ANESTOR_PPU_H
