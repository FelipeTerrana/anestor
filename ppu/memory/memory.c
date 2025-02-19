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
#define SPRITE_ZERO_HIT_MASK 0x40
#define IN_VBLANK_MASK 0x80

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
    uint8_t readBuffer;
    uint8_t addressMiddleValue;
    bool nmi;
};


PpuMemory* ppuMemoryInit(Cartridge* cartridge)
{
    PpuMemory* memory = malloc( sizeof(struct ppu_memory__) );

    memory->patternTables = patternTablesInit(cartridge);
    memory->nametables = nametablesInit(cartridge);
    memory->paletteRam = paletteRamInit();
    memory->ppustatus = 0;
    memory->ppuctrl = memory->ppumask = memory->oamaddr = memory->ppuaddr = 0;
    memory->ppuscrollX = memory->ppuscrollY = memory->addressLatch = memory->readBuffer = 0;
    memory->addressMiddleValue = 0;
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
    uint8_t read = memory->readBuffer;

    if(memory->ppuaddr >= PPU_CARTRIDGE_SPACE_FIRST_ADDRESS && memory->ppuaddr <= PPU_CARTRIDGE_SPACE_LAST_ADDRESS)
        memory->readBuffer = patternTablesRead(memory->patternTables, memory->ppuaddr);

    else if(memory->ppuaddr >= NAMETABLE_SPACE_FIRST_ADDRESS && memory->ppuaddr <= NAMETABLE_SPACE_LAST_ADDRESS)
        memory->readBuffer = nametablesRead(memory->nametables, memory->ppuaddr);

    else if(memory->ppuaddr >= PALLETTE_RAM_FIRST_ADDRESS && memory->ppuaddr <= PALLETTE_RAM_LAST_ADDRESS)
        read = memory->readBuffer = paletteRamRead(memory->paletteRam, memory->ppuaddr % PALLETTE_RAM_SIZE);

    incrementVramAddress__(memory);
    return read;
}



static bool ppuMemoryWrite__(PpuMemory* memory, uint8_t value)
{
    bool written = false;

    setFlagValue(&memory->ppustatus, 0x1F, value);

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
                memory->addressMiddleValue = value;
                memory->addressLatch = ~((uint8_t) 0);
            }
            else
            {
                memory->ppuaddr = ((uint16_t) memory->addressMiddleValue << 8u) | value;
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



void ppuRegistersSetSpriteZeroHit(PpuMemory* memory)
{
    setFlagValue(&memory->ppustatus, SPRITE_ZERO_HIT_MASK, 1);
}



void ppuRegistersClearSpriteZeroHit(PpuMemory* memory)
{
    setFlagValue(&memory->ppustatus, SPRITE_ZERO_HIT_MASK, 0);
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
    uint8_t patternTableNumber;
    NesPixel renderBuffer[64];
    Palette palette;

    patternTableNumber = getFlagValue(memory->ppuctrl, BACKGROUND_PATTERN_TABLE_MASK);

    for(tileOffsetY = 0; tileOffsetY < TOTAL_TILE_ROWS; tileOffsetY++)
    {
        for(tileOffsetX = 0; tileOffsetX < TOTAL_TILE_COLS; tileOffsetX++)
        {
            paletteRamGetPalette(memory->paletteRam, nametablesGetTilePaletteNumber(memory->nametables, tileOffsetX, tileOffsetY), BACKGROUND, &palette);
            patternTablesRenderTile(memory->patternTables, nametablesGetTileIndex(memory->nametables, tileOffsetX, tileOffsetY),
                                    patternTableNumber, &palette, renderBuffer);

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
    Sprite sprite;
    Palette palette;
    NesPixel renderBuffer[64];
    bool spriteZeroHit;

    for(spriteIndex = 0; spriteIndex < NUMBER_OF_SPRITES; spriteIndex++)
    {
        spriteInit(&sprite, spriteIndex, memory->oam);

        paletteRamGetPalette(memory->paletteRam, spriteGetPaletteNumber(&sprite), SPRITE, &palette);

        patternTablesRenderTile(memory->patternTables, spriteGetTileNumber(&sprite),
                                patternTableNumber, &palette, renderBuffer);

        for(i=0; i < 64; i++)
        {
            uint8_t pixelOffsetX = i % 8;
            uint8_t pixelOffsetY = i / 8;

            if(spriteIsFlippedHorizontally(&sprite))
                pixelOffsetX = 7 - pixelOffsetX;

            if(spriteIsFlippedVertically(&sprite))
                pixelOffsetY = 7 - pixelOffsetY;

            renderBuffer[i].type = SPRITE;
            renderBuffer[i].priority = spriteIndex;
            renderBuffer[i].isBehindBackground = spriteIsBehindBackground(&sprite);

            spriteZeroHit = screenSetPixel(screen,
                                           spriteGetXPosition(&sprite) + pixelOffsetX,
                                           spriteGetYPosition(&sprite) + pixelOffsetY,
                                           &renderBuffer[i]);

            if(spriteZeroHit)
                ppuRegistersSetSpriteZeroHit(memory);
        }
    }
}



void ppuMemoryRender(PpuMemory* memory, Screen* screen)
{
    screenSetScroll(screen,
                    memory->ppuscrollX + NATIVE_WIDTH * getFlagValue(memory->ppuctrl, X_SCROLL_MSB_MASK),
                    memory->ppuscrollY + NATIVE_HEIGHT * getFlagValue(memory->ppuctrl, Y_SCROLL_MSB_MASK));

    screenSetPpumask(screen, memory->ppumask);

    renderBackground(memory, screen);
    renderSprites(memory, screen);

    screenRefresh(screen);
}
