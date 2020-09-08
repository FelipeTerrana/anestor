#include "memory.h"

#include <stdint.h>
#include <stdlib.h>
#include "../flag_ops.h"

#define VRAM_SIZE 0x0800
#define PALETTE_RAM_SIZE 0x0020
#define OAM_SIZE 0x0100

#define PPUCTRL_CPU_ADDRESS     0x2000 // w
#define PPUMASK_CPU_ADDRESS     0x2001 // w
#define PPUSTATUS_CPU_ADDRESS   0x2002 // r
#define OAMADDR_CPU_ADDRESS     0x2003 // w
#define OAMDATA_CPU_ADDRESS     0x2004 // rw
#define PPUSCROLL_CPU_ADDRESS   0x2005 // w2
#define PPUADDR_CPU_ADDRESS     0x2006 // w2
#define PPUDATA_CPU_ADDRESS     0x2007 // rw

#define IN_VBLANK_MASK 0x80

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
    // TODO ppuMemoryRead__
    return 0x0;
}



static bool ppuMemoryWrite__(PpuMemory* memory, uint8_t value)
{
    // TODO ppuMemoryWrite__
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