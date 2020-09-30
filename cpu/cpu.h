#ifndef ANESTOR_CPU_H
#define ANESTOR_CPU_H

#include <stdbool.h>
#include "instructions.h"
#include "memory.h"
#include "../cartridge/cartridge.h"
#include "apu/apu.h"
#include "../ppu/memory/memory.h"

typedef struct cpu__ Cpu;

Cpu* cpuInit(PpuMemory* ppuMemory, Cartridge* cartridge);
void cpuShutdown(Cpu* cpu);

int cpuLoop(void* data);

//void testDecoder(uint8_t opcode);


#endif //ANESTOR_CPU_H
