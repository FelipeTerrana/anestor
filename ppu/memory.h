#ifndef ANESTOR_PPU_MEMORY_H
#define ANESTOR_PPU_MEMORY_H

#include "../cartridge.h"

typedef struct ppu_memory__ PpuMemory;

PpuMemory* ppuMemoryInit(Cartridge* cartridge);
void ppuMemoryShutdown(PpuMemory* memory);

#endif //ANESTOR_PPU_MEMORY_H
