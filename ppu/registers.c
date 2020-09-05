#include "registers.h"

#include <stdint.h>
#include <stdlib.h>

struct ppu_registers__ {
    uint8_t ppuctrl;
    uint8_t ppumask;
    uint8_t ppustatus;
    uint8_t oamaddr;
    uint8_t ppuscrollX;
    uint8_t ppuscrollY;
    uint16_t ppuaddr;
    uint8_t oamdma;
};


PpuRegisters* ppuRegistersInit()
{
    PpuRegisters* registers = malloc( sizeof(struct ppu_registers__) );

    registers->ppuctrl = registers->ppumask = registers->ppustatus = 0;
    registers->oamaddr = registers->ppuaddr = 0;

    return registers;
}



void ppuRegistersShutdown(PpuRegisters* registers)
{
    free(registers);
}