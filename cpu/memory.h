#ifndef ANESTOR_MEMORY_H
#define ANESTOR_MEMORY_H

#include <stdint.h>
#include "../cartridge.h"

typedef struct cpu_memory__ CpuMemory;

CpuMemory* cpuMemoryInit(/* TODO ponteiro pra PPU / APU */Cartridge* cartridge);
void cpuMemoryShutdown(CpuMemory* memory);

#endif //ANESTOR_MEMORY_H
