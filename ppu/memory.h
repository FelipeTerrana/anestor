#ifndef ANESTOR_PPU_MEMORY_H
#define ANESTOR_PPU_MEMORY_H

#include <stdbool.h>
#include <stdint.h>
#include "../cartridge/cartridge.h"

typedef struct ppu_memory__ PpuMemory;

PpuMemory* ppuMemoryInit(Cartridge* cartridge);
void ppuMemoryShutdown(PpuMemory* memory);

uint8_t ppuRegistersRead(PpuMemory* memory, uint16_t address);
bool ppuRegistersWrite(PpuMemory* memory, uint16_t address, uint8_t value);

void ppuMemoryOamWrite(PpuMemory* memory, uint8_t oamAddress, uint8_t value);

#endif //ANESTOR_PPU_MEMORY_H
