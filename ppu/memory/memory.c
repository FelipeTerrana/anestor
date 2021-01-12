#include "memory.h"

#include <stdint.h>
#include <stdlib.h>
#include "../../flag_ops.h"
#include "pattern_tables.h"
#include "nametables.h"
#include "palette/palette_ram.h"
#include "sprite.h"

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

#define PALLETTE_RAM_FIRST_ADDRESS 0x3F00
#define PALLETTE_RAM_LAST_ADDRESS  0x3FFF

// PPUCTRL
#define X_SCROLL_MSB_MASK 0x01
#define Y_SCROLL_MSB_MASK 0x02
#define VRAM_ADDRESS_INCREMENT_MASK 0x04
#define SPRITE_PATTERN_TABLE_MASK 0x08
#define BACKGROUND_PATTERN_TABLE_MASK 0x10
#define GENERATE_NMI_ON_VBLANK_MASK 0x80

// PPUSTATUS
#define IN_VBLANK_MASK 0x80

#define TOTAL_TILE_ROWS 60
#define TOTAL_TILE_COLS 64

struct ppu_memory__ {
    PatternTables* patternTables;
    Nametables* nametables;
    PaletteRam* paletteRam;
    uint8_t oam[OAM_SIZE];

    uint8_t ppuctrl;
    uint8_t ppumask;
    uint8_t ppustatus;
    uint8_t oamaddr;
    uint8_t ppuscrollX;
    uint8_t ppuscrollY;
    uint16_t ppuaddr;

    uint8_t addressLatch;
    bool nmi;
};


PpuMemory* ppuMemoryInit(Cartridge* cartridge)
{
    PpuMemory* memory = malloc( sizeof(struct ppu_memory__) );

    memory->patternTables = patternTablesInit(cartridge);
    memory->nametables = nametablesInit(cartridge);
    memory->paletteRam = paletteRamInit();
    memory->ppustatus = (1u << 5u) | (1u << 7u);
    memory->ppuctrl = memory->ppumask = memory->oamaddr = memory->ppuaddr = 0;
    memory->ppuscrollX = memory->ppuscrollY = memory->addressLatch = 0;
    memory->nmi = false;

    return memory;
}



void ppuMemoryShutdown(PpuMemory* memory)
{
    patternTablesShutdown(memory->patternTables);
    nametablesShutdown(memory->nametables);
    paletteRamShutdown(memory->paletteRam);

    free(memory);
}



static void incrementVramAddress__(PpuMemory* memory)
{
    memory->ppuaddr += getFlagValue(memory->ppuctrl, VRAM_ADDRESS_INCREMENT_MASK) == 0 ? 1 : 32;
}



static uint8_t ppuMemoryRead__(PpuMemory* memory)
{
    uint8_t read = 0x0;

    if(memory->ppuaddr >= PPU_CARTRIDGE_SPACE_FIRST_ADDRESS && memory->ppuaddr <= PPU_CARTRIDGE_SPACE_LAST_ADDRESS)
        read = patternTablesRead(memory->patternTables, memory->ppuaddr);

    else if(memory->ppuaddr >= NAMETABLE_SPACE_FIRST_ADDRESS && memory->ppuaddr <= NAMETABLE_SPACE_LAST_ADDRESS)
        read = nametablesRead(memory->nametables, memory->ppuaddr);

    else if(memory->ppuaddr >= PALLETTE_RAM_FIRST_ADDRESS && memory->ppuaddr <= PALLETTE_RAM_LAST_ADDRESS)
        read = paletteRamRead(memory->paletteRam, memory->ppuaddr % PALLETTE_RAM_SIZE);

    incrementVramAddress__(memory);
    return read;
}



static bool ppuMemoryWrite__(PpuMemory* memory, uint8_t value)
{
    bool written = false;

    if(memory->ppuaddr >= PPU_CARTRIDGE_SPACE_FIRST_ADDRESS && memory->ppuaddr <= PPU_CARTRIDGE_SPACE_LAST_ADDRESS)
        written = patternTablesWrite(memory->patternTables, memory->ppuaddr, value);

    else if(memory->ppuaddr >= NAMETABLE_SPACE_FIRST_ADDRESS && memory->ppuaddr <= NAMETABLE_SPACE_LAST_ADDRESS)
        written = nametablesWrite(memory->nametables, memory->ppuaddr, value);

    else if(memory->ppuaddr >= PALLETTE_RAM_FIRST_ADDRESS && memory->ppuaddr <= PALLETTE_RAM_LAST_ADDRESS)
        written = paletteRamWrite(memory->paletteRam, memory->ppuaddr % PALLETTE_RAM_SIZE, value);

    incrementVramAddress__(memory);
    return written;
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
            break;

        case PPUDATA_CPU_ADDRESS:
            read = ppuMemoryRead__(memory);
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
            {
                memory->ppuscrollX = value;
                memory->addressLatch = ~((uint8_t) 0);
            }
            else
            {
                memory->ppuscrollY = value;
                memory->addressLatch = 0;
            }
            written = true;
            memory->addressLatch = ~((uint8_t) 0);
            break;

        case PPUADDR_CPU_ADDRESS:
            if (memory->addressLatch == 0)
            {
                memory->ppuaddr = (uint16_t) value << 8u;
                memory->addressLatch = ~((uint8_t) 0);
            }
            else
            {
                memory->ppuaddr |= value;
                memory->addressLatch = 0;
            }
            written = true;
            break;

        case PPUDATA_CPU_ADDRESS:
            written = ppuMemoryWrite__(memory, value);
            break;
    }

    return written;
}



void ppuRegistersStartVblank(PpuMemory* memory)
{
    setFlagValue(&memory->ppustatus, IN_VBLANK_MASK, 1);
    memory->nmi = getFlagValue(memory->ppuctrl, GENERATE_NMI_ON_VBLANK_MASK) == 1 ? true : false;
}



void ppuRegistersStopVblank(PpuMemory* memory)
{
    setFlagValue(&memory->ppustatus, IN_VBLANK_MASK, 0);
    memory->nmi = false;
}



bool ppuMemoryGetNMI(PpuMemory* memory)
{
    bool old = memory->nmi;
    memory->nmi = false;

    return old;
}



void ppuMemoryOamWrite(PpuMemory* memory, uint8_t oamAddress, uint8_t value)
{
    memory->oam[oamAddress] = value;
}



static void renderBackground(PpuMemory* memory, Screen* screen)
{
    uint8_t tileOffsetX, tileOffsetY;
    uint8_t i;
    uint8_t nametableNumber, patternTableNumber;
    uint8_t attributeByte, attributeMask;
    uint16_t tileAddress, attributeAddress, addressOffset;
    NesPixel renderBuffer[64];
    Palette* palette;

    patternTableNumber = getFlagValue(memory->ppuctrl, BACKGROUND_PATTERN_TABLE_MASK);

    for(nametableNumber = 0; nametableNumber < NUMBER_OF_NAMETABLES; nametableNumber++)
    {
        for (addressOffset = 0; addressOffset < TILES_PER_NAMETABLE; addressOffset++)
        {
            tileAddress = NAMETABLE_SPACE_FIRST_ADDRESS + nametableNumber * NAMETABLE_SIZE + addressOffset;

            tileOffsetX = addressOffset % 0x20 + (nametableNumber % 2) * (TOTAL_TILE_COLS / 2);
            tileOffsetY = addressOffset / 0x20 + (nametableNumber / 2) * (TOTAL_TILE_ROWS / 2);

            attributeAddress = NAMETABLE_SPACE_FIRST_ADDRESS + nametableNumber * NAMETABLE_SIZE +
                               ((tileOffsetX / 4) + (tileOffsetY / 4) * 8) + TILES_PER_NAMETABLE;

            attributeMask = 3 << (2 * ((tileOffsetX % 4) / 2 + 2 * ((tileOffsetY % 4) / 2)));
            attributeByte = nametablesRead(memory->nametables, attributeAddress);
            palette = paletteRamGetPalette(memory->paletteRam, getFlagValue(attributeByte, attributeMask), BACKGROUND);

            patternTablesRenderTile(memory->patternTables, nametablesRead(memory->nametables, tileAddress),
                                    patternTableNumber, palette, renderBuffer);

            paletteShutdown(palette);

            for(i=0; i < 64; i++)
            {
                renderBuffer[i].type = BACKGROUND;

                screenSetPixel(screen,
                               (tileOffsetX * 8) + (i % 8),
                               (tileOffsetY * 8) + (i / 8),
                               &renderBuffer[i]);
            }
        }
    }
}



static void renderSprites(PpuMemory* memory, Screen* screen)
{
    uint8_t spriteIndex, i;
    uint8_t patternTableNumber = getFlagValue(memory->ppuctrl, SPRITE_PATTERN_TABLE_MASK);
    Palette* palette;
    NesPixel renderBuffer[64];

    for(spriteIndex = 0; spriteIndex < NUMBER_OF_SPRITES; spriteIndex++)
    {
        Sprite* sprite = spriteInit(spriteIndex, memory->oam);

        palette = paletteRamGetPalette(memory->paletteRam, spriteGetPaletteNumber(sprite), SPRITE);

        patternTablesRenderTile(memory->patternTables, spriteGetTileNumber(sprite),
                                patternTableNumber, palette, renderBuffer);

        for(i=0; i < 64; i++)
        {
            uint8_t pixelOffsetX = i % 8;
            uint8_t pixelOffsetY = i / 8;

            if(spriteIsFlippedHorizontally(sprite))
                pixelOffsetX = 7 - pixelOffsetX;

            if(spriteIsFlippedVertically(sprite))
                pixelOffsetY = 7 - pixelOffsetY;

            renderBuffer[i].type = SPRITE;
            renderBuffer[i].priority = spriteIndex;
            renderBuffer[i].isBehindBackground = spriteIsBehindBackground(sprite);

            screenSetPixel(screen,
                           spriteGetXPosition(sprite) + pixelOffsetX,
                           spriteGetYPosition(sprite) + pixelOffsetY,
                           &renderBuffer[i]);
        }

        paletteShutdown(palette);
        spriteShutdown(sprite);
    }
}



void ppuMemoryRender(PpuMemory* memory, Screen* screen)
{
    screenSetScroll(screen,
                    memory->ppuscrollX + NATIVE_WIDTH * getFlagValue(memory->ppuctrl, X_SCROLL_MSB_MASK),
                    memory->ppuscrollY + NATIVE_HEIGHT * getFlagValue(memory->ppuctrl, Y_SCROLL_MSB_MASK));

    screenSetPpumask(screen, memory->ppumask);

    renderSprites(memory, screen);
    renderBackground(memory, screen);

    screenRefresh(screen);
}
