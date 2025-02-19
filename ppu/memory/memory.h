#ifndef ANESTOR_PPU_MEMORY_H
#define ANESTOR_PPU_MEMORY_H

#include <stdbool.h>
#include <stdint.h>
#include "../../cartridge/cartridge.h"
#include "../screen.h"

typedef struct ppu_memory__ PpuMemory;

PpuMemory* ppuMemoryInit(Cartridge* cartridge);
void ppuMemoryShutdown(PpuMemory* memory);

uint8_t ppuRegistersRead(PpuMemory* memory, uint16_t address);
bool ppuRegistersWrite(PpuMemory* memory, uint16_t address, uint8_t value);

void ppuRegistersStartVblank(PpuMemory* memory);
void ppuRegistersStopVblank(PpuMemory* memory);

void ppuRegistersSetSpriteZeroHit(PpuMemory* memory);
void ppuRegistersClearSpriteZeroHit(PpuMemory* memory);

bool ppuMemoryGetNMI(PpuMemory* memory);

void ppuMemoryOamWrite(PpuMemory* memory, uint8_t oamAddress, uint8_t value);

void ppuMemoryRender(PpuMemory* memory, Screen* screen);

#endif //ANESTOR_PPU_MEMORY_H
