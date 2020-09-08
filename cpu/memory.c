#include "memory.h"

#include <stdlib.h>

#define CPU_RAM_SIZE 2048

#define CPU_RAM_FIRST_ADDRESS 0x0000
#define CPU_RAM_LAST_ADDRESS  0x1FFF

#define PPU_REGISTERS_FIRST_ADDRESS 0x2000
#define PPU_REGISTERS_LAST_ADDRESS  0x3FFF

#define RP2A03_REGISTERS_FIRST_ADDRESS 0x4000
#define RP2A03_REGISTERS_LAST_ADDRESS  0x401F

#define CARTRIDGE_SPACE_FIRST_ADDRESS 0x4020
#define CARTRIDGE_SPACE_LAST_ADDRESS  0xFFFF

#define OAMDMA_ADDRESS 0x4014

struct cpu_memory__ {
    uint8_t ram[CPU_RAM_SIZE];
    uint16_t pc;
    PpuMemory* ppuMemory;
    Apu* apu;
    Cartridge* cartridge;
};



CpuMemory* cpuMemoryInit(PpuMemory* ppuMemory, Apu* apu, Cartridge* cartridge)
{
    CpuMemory* memory = malloc( sizeof(struct cpu_memory__) );

    memory->ppuMemory = ppuMemory;
    memory->apu = apu;
    memory->cartridge = cartridge;

    return memory;
}



void cpuMemoryShutdown(CpuMemory* memory)
{
    free(memory);
}



uint16_t cpuMemoryRead(CpuMemory* memory, uint16_t address, uint8_t* value)
{
    if(address >= CPU_RAM_FIRST_ADDRESS && address <= CPU_RAM_LAST_ADDRESS)
    {
        *value = memory->ram[address % CPU_RAM_SIZE];
        return 0;
    }

    else if(address >= PPU_REGISTERS_FIRST_ADDRESS && address <= PPU_REGISTERS_LAST_ADDRESS)
    {
        *value = ppuRegistersRead(memory->ppuMemory, PPU_REGISTERS_FIRST_ADDRESS + address % 8);
        return 0;
    }

    else if(address >= RP2A03_REGISTERS_FIRST_ADDRESS && address <= RP2A03_REGISTERS_LAST_ADDRESS)
    {
        return 0; // TODO implement 2A03 registers read
    }

    else if(address >= CARTRIDGE_SPACE_FIRST_ADDRESS && address <= CARTRIDGE_SPACE_LAST_ADDRESS)
    {
        *value = cartridgeRead(memory->cartridge, address);
        return 0;
    }

    else
        return 0;
}



uint16_t cpuMemoryWrite(CpuMemory* memory, uint16_t address, uint8_t value)
{
    if(address >= CPU_RAM_FIRST_ADDRESS && address <= CPU_RAM_LAST_ADDRESS)
    {
        memory->ram[address % CPU_RAM_SIZE] = value;
        return 0;
    }

    else if(address >= PPU_REGISTERS_FIRST_ADDRESS && address <= PPU_REGISTERS_LAST_ADDRESS)
    {
        ppuRegistersWrite(memory->ppuMemory, PPU_REGISTERS_FIRST_ADDRESS + address % 8, value);
        return 0;
    }

    else if(address == OAMDMA_ADDRESS)
    {
        uint16_t oamAddress;
        uint8_t oamValue;
        for (oamAddress = 0; oamAddress < 0x100; oamAddress++)
        {
            cpuMemoryRead(memory, (value << 8u) | oamAddress, &oamValue);
            ppuMemoryOamWrite(memory->ppuMemory, oamAddress, oamValue);
        }

        return 513 + rand() % 2; // TODO make wait time more accurate
    }

    else if(address >= RP2A03_REGISTERS_FIRST_ADDRESS && address <= RP2A03_REGISTERS_LAST_ADDRESS)
    {
        return 0; // TODO implement 2A03 registers write
    }

    else if(address >= CARTRIDGE_SPACE_FIRST_ADDRESS && address <= CARTRIDGE_SPACE_LAST_ADDRESS)
    {
        cartridgeWrite(memory->cartridge, address, value);
        return 0;
    }

    else
        return 0;
}



uint8_t cpuMemoryFetchInstruction(CpuMemory* memory)
{
    uint8_t instruction;
    cpuMemoryRead(memory, memory->pc++, &instruction);

    return instruction;
}