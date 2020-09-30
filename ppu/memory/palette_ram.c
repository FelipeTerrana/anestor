#include "palette_ram.h"

#include <stdlib.h>

struct palette_ram__ {
    uint8_t ram[PALLETTE_RAM_SIZE];
};



PaletteRam* paletteRamInit()
{
    PaletteRam* paletteRam = malloc( sizeof(struct palette_ram__) );
    return paletteRam;
}



void paletteRamShutdown(PaletteRam* paletteRam)
{
    free(paletteRam);
}



uint8_t paletteRamRead(PaletteRam* paletteRam, uint16_t address)
{
    return paletteRam->ram[address % PALLETTE_RAM_SIZE];
}



bool paletteRamWrite(PaletteRam* paletteRam, uint16_t address, uint8_t value)
{
    paletteRam->ram[address % PALLETTE_RAM_SIZE] = value;
    return true;
}



Palette paletteRamGetPalette(PaletteRam* paletteRam, uint8_t paletteNumber, enum PaletteType type)
{
    uint16_t index = (paletteNumber << 2u) + (type << 4u);
    return &paletteRam->ram[index];
}



NesPixel paletteRamGetBackgroundColor(PaletteRam* paletteRam)
{
    return paletteRam->ram[0];
}