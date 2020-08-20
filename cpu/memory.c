#include "memory.h"

#include <stdlib.h>

#define CPU_RAM_SIZE 0x0800

#define PPU_REGISTERS_START 0x2000
#define PPU_REGISTERS_SIZE 0x0008

#define APU_IO_REGISTERS_START 0x4000
#define APU_IO_TEST_REGISTERS_START 0x4018
#define CPU_CARTRIDGE_SPACE_START 0x4020

struct cpu_memory__ {
    uint8_t ram[CPU_RAM_SIZE];
    uint16_t pc;
    // TODO ponteiro pra PPU
    // TODO ponteiro pra APU
    Cartridge* cartridge;
};



CpuMemory* cpuMemoryInit(/* TODO ponteiro pra PPU / APU */Cartridge* cartridge)
{
    CpuMemory* memory = malloc( sizeof(struct cpu_memory__) );

    // TODO ponteiro pra PPU
    // TODO ponteiro pra APU
    memory->cartridge = cartridge;

    return memory;
}



void cpuMemoryShutdown(CpuMemory* memory)
{
    free(memory);
}



uint8_t cpuMemoryRead(CpuMemory* memory, uint16_t address)
{
    // TODO cpuMemoryRead
    return 0x0;
}



bool cpuMemoryWrite(CpuMemory* memory, uint16_t address, uint8_t value)
{
    // TODO cpuMemoryWrite
    return false;
}



uint8_t cpuMemoryFetchInstruction(CpuMemory* memory)
{
    return cpuMemoryRead(memory, memory->pc++);
}