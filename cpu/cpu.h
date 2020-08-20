#ifndef ANESTOR_CPU_H
#define ANESTOR_CPU_H

#include <stdbool.h>
#include "instructions.h"
#include "memory.h"
#include "../cartridge.h"

typedef struct cpu__ Cpu;

Cpu* cpuInit(/* TODO ponteiro pra PPU / APU */Cartridge* cartridge);
void cpuShutdown(Cpu* cpu);

int cpuLoop(void* stopSignal);

//void testDecoder(uint8_t opcode);


#endif //ANESTOR_CPU_H
