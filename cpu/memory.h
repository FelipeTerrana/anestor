#ifndef ANESTOR_CPU_MEMORY_H
#define ANESTOR_CPU_MEMORY_H

#include <stdint.h>
#include <stdbool.h>
#include "../cartridge.h"
#include "apu/apu.h"
#include "../ppu/memory.h"

typedef struct cpu_memory__ CpuMemory;

CpuMemory* cpuMemoryInit(PpuMemory* ppuMemory, Apu* apu, Cartridge* cartridge);
void cpuMemoryShutdown(CpuMemory* memory);

uint8_t cpuMemoryRead(CpuMemory* memory, uint16_t address);
bool cpuMemoryWrite(CpuMemory* memory, uint16_t address, uint8_t value);

uint8_t cpuMemoryFetchInstruction(CpuMemory* memory);

#endif //ANESTOR_CPU_MEMORY_H
