#include "memory.h"

#include <stdint.h>
#include <stdlib.h>

#define VRAM_SIZE 0x0800
#define PALETTE_RAM_SIZE 0x0020
#define OAM_SIZE 0x0100

#define PPUCTRL_CPU_ADDRESS     0x2000
#define PPUMASK_CPU_ADDRESS     0x2001
#define PPUSTATUS_CPU_ADDRESS   0x2002
#define OAMADDR_CPU_ADDRESS     0x2003
#define OAMDATA_CPU_ADDRESS     0x2004
#define PPUSCROLL_CPU_ADDRESS   0x2005
#define PPUADDR_CPU_ADDRESS     0x2006
#define PPUDATA_CPU_ADDRESS     0x2007
#define OAMDMA_CPU_ADDRESS      0x4014

struct ppu_memory__ {
    Cartridge* cartridge;
    uint8_t vram[VRAM_SIZE];
    uint8_t paletteRam[PALETTE_RAM_SIZE];
    uint8_t oam[OAM_SIZE];

    uint8_t ppuctrl;
    uint8_t ppumask;
    uint8_t ppustatus;
    uint8_t oamaddr;
    uint8_t ppuscrollX;
    uint8_t ppuscrollY;
    uint16_t ppuaddr;
    uint8_t oamdma;
};


PpuMemory* ppuMemoryInit(Cartridge* cartridge)
{
    PpuMemory* memory = malloc( sizeof(struct ppu_memory__) );

    memory->cartridge = cartridge;
    memory->ppuctrl = memory->ppumask = memory->ppustatus = 0;
    memory->oamaddr = memory->ppuaddr = 0;

    return memory;
}



void ppuMemoryShutdown(PpuMemory* memory)
{
    free(memory);
}