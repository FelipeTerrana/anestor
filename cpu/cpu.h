#ifndef ANESTOR_CPU_H
#define ANESTOR_CPU_H

#include <stdbool.h>
#include "instructions.h"
#include "memory.h"
#include "../cartridge.h"
#include "apu/apu.h"
#include "../ppu/memory.h"

#define NES_CPU_CLOCKS_PER_SEC 1790

typedef struct cpu__ Cpu;

Cpu* cpuInit(PpuMemory* ppuMemory, Cartridge* cartridge);
void cpuShutdown(Cpu* cpu);

int cpuLoop(void* stopSignal);

//void testDecoder(uint8_t opcode);


#endif //ANESTOR_CPU_H
