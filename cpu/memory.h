#ifndef ANESTOR_CPU_MEMORY_H
#define ANESTOR_CPU_MEMORY_H

#include <stdint.h>
#include <stdbool.h>
#include "../cartridge/cartridge.h"
#include "apu/apu.h"
#include "../ppu/memory/memory.h"

#define NMI_VECTOR_ADDRESS 0xFFFA
#define RESET_VECTOR_ADDRESS 0xFFFC
#define IRQ_VECTOR_ADDRESS 0xFFFE

typedef struct cpu_memory__ CpuMemory;

CpuMemory* cpuMemoryInit(PpuMemory* ppuMemory, Apu* apu, Cartridge* cartridge);
void cpuMemoryShutdown(CpuMemory* memory);

uint16_t cpuMemoryRead(CpuMemory* memory, uint16_t address, uint8_t* value);
uint16_t cpuMemoryWrite(CpuMemory* memory, uint16_t address, uint8_t value);

uint16_t cpuMemoryRead16(CpuMemory* memory, uint16_t address, uint16_t* value);

uint8_t cpuMemoryFetchInstruction(CpuMemory* memory);
void cpuMemoryJump(CpuMemory* memory, uint16_t address);
uint16_t cpuMemoryGetPc(CpuMemory* memory);
uint16_t cpuMemoryBranch(CpuMemory* memory, int8_t offset);

#endif //ANESTOR_CPU_MEMORY_H
