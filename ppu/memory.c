#include "memory.h"

#include <stdint.h>
#include <stdlib.h>
#include "../flag_ops.h"

#define VRAM_SIZE 0x0800
#define PALLETTE_RAM_SIZE 0x0020
#define OAM_SIZE 0x0100

#define PPUCTRL_CPU_ADDRESS     0x2000 // w
#define PPUMASK_CPU_ADDRESS     0x2001 // w
#define PPUSTATUS_CPU_ADDRESS   0x2002 // r
#define OAMADDR_CPU_ADDRESS     0x2003 // w
#define OAMDATA_CPU_ADDRESS     0x2004 // rw
#define PPUSCROLL_CPU_ADDRESS   0x2005 // w2
#define PPUADDR_CPU_ADDRESS     0x2006 // w2
#define PPUDATA_CPU_ADDRESS     0x2007 // rw

#define PPU_CARTRIDGE_SPACE_FIRST_ADDRESS 0x0000
#define PPU_CARTRIDGE_SPACE_LAST_ADDRESS  0x1FFF

#define NAMETABLE_SPACE_FIRST_ADDRESS 0x2000
#define NAMETABLE_SPACE_LAST_ADDRESS  0x3EFF

#define NAMETABLE_SPACE_SIZE 0x1000

#define PALLETTE_RAM_FIRST_ADDRESS 0x3F00
#define PALLETTE_RAM_LAST_ADDRESS  0x3FFF

#define IN_VBLANK_MASK 0x80

struct ppu_memory__ {
    Cartridge* cartridge;
    uint8_t vram[VRAM_SIZE];
    uint8_t paletteRam[PALLETTE_RAM_SIZE];
    uint8_t oam[OAM_SIZE];

    uint8_t ppuctrl;
    uint8_t ppumask;
    uint8_t ppustatus;
    uint8_t oamaddr;
    uint8_t ppuscrollX;
    uint8_t ppuscrollY;
    uint16_t ppuaddr;

    uint8_t addressLatch;
};


PpuMemory* ppuMemoryInit(Cartridge* cartridge)
{
    PpuMemory* memory = malloc( sizeof(struct ppu_memory__) );

    memory->cartridge = cartridge;
    memory->ppustatus = (1u << 5u) | (1u << 7u);
    memory->ppuctrl = memory->ppumask = memory->oamaddr = memory->ppuaddr = 0;
    memory->addressLatch = 0;

    return memory;
}



void ppuMemoryShutdown(PpuMemory* memory)
{
    free(memory);
}



static uint8_t ppuMemoryRead__(PpuMemory* memory)
{
    if(memory->ppuaddr >= PPU_CARTRIDGE_SPACE_FIRST_ADDRESS && memory->ppuaddr <= PPU_CARTRIDGE_SPACE_LAST_ADDRESS)
        return cartridgeRead(memory->cartridge, memory->ppuaddr);

    else if(memory->ppuaddr >= NAMETABLE_SPACE_FIRST_ADDRESS && memory->ppuaddr <= NAMETABLE_SPACE_LAST_ADDRESS)
        return memory->vram[memory->ppuaddr % NAMETABLE_SPACE_SIZE]; // TODO support cartridge-space nametables

    else if(memory->ppuaddr >= PALLETTE_RAM_FIRST_ADDRESS && memory->ppuaddr <= PALLETTE_RAM_LAST_ADDRESS)
        return memory->paletteRam[memory->ppuaddr % PALLETTE_RAM_SIZE];

    return 0x0;
}



static bool ppuMemoryWrite__(PpuMemory* memory, uint8_t value)
{
    if(memory->ppuaddr >= PPU_CARTRIDGE_SPACE_FIRST_ADDRESS && memory->ppuaddr <= PPU_CARTRIDGE_SPACE_LAST_ADDRESS)
        return cartridgeWrite(memory->cartridge, memory->ppuaddr, value);

    else if(memory->ppuaddr >= NAMETABLE_SPACE_FIRST_ADDRESS && memory->ppuaddr <= NAMETABLE_SPACE_LAST_ADDRESS)
    {
        memory->vram[memory->ppuaddr % NAMETABLE_SPACE_SIZE] = value;
        return true; // TODO support cartridge-space nametables
    }

    else if(memory->ppuaddr >= PALLETTE_RAM_FIRST_ADDRESS && memory->ppuaddr <= PALLETTE_RAM_LAST_ADDRESS)
    {
        memory->paletteRam[memory->ppuaddr % PALLETTE_RAM_SIZE] = value;
        return true;
    }

    return false;
}



uint8_t ppuRegistersRead(PpuMemory* memory, uint16_t address)
{
    uint8_t read;
    switch (address)
    {
        case PPUSTATUS_CPU_ADDRESS:
            read = memory->ppustatus;
            setFlagValue(&memory->ppustatus, IN_VBLANK_MASK, 0);
            memory->addressLatch = 0;
            break;

        case OAMDATA_CPU_ADDRESS:
            read = memory->oam[memory->oamaddr];
            memory->addressLatch = read;
            break;

        case PPUDATA_CPU_ADDRESS:
            read = ppuMemoryRead__(memory);
            memory->addressLatch = read;
            break;

        default:
            read = memory->addressLatch;
    }

    return read;
}



bool ppuRegistersWrite(PpuMemory* memory, uint16_t address, uint8_t value)
{
    bool written;
    switch (address)
    {
        case PPUCTRL_CPU_ADDRESS:
            memory->ppuctrl = value;
            written = true;
            break;

        case PPUMASK_CPU_ADDRESS:
            memory->ppumask = value;
            written = true;
            break;

        case OAMADDR_CPU_ADDRESS:
            memory->oamaddr = value;
            written = true;
            break;

        case OAMDATA_CPU_ADDRESS:
            memory->oam[memory->oamaddr++] = value;
            written = true;
            break;

        case PPUSCROLL_CPU_ADDRESS:
            if (memory->addressLatch == 0)
                memory->ppuscrollX = value;
            else
                memory->ppuscrollY = value;
            written = true;
            break;

        case PPUADDR_CPU_ADDRESS:
            if (memory->addressLatch == 0)
                memory->ppuaddr = (uint16_t) value << 8u;
            else
                memory->ppuaddr |= value;
            written = true;
            break;

        case PPUDATA_CPU_ADDRESS:
            written = ppuMemoryWrite__(memory, value);
            break;
    }

    memory->addressLatch = ~((uint8_t) 0);
    return written;
}



void ppuMemoryOamWrite(PpuMemory* memory, uint8_t oamAddress, uint8_t value)
{
    memory->oam[oamAddress] = value;
}