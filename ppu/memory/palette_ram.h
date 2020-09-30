#ifndef ANESTOR_PALETTE_RAM_H
#define ANESTOR_PALETTE_RAM_H

#include <stdint.h>
#include <stdbool.h>
#include "../screen.h"

#define PALLETTE_RAM_SIZE 0x0020

typedef const uint8_t* Palette;

typedef struct palette_ram__ PaletteRam;

enum PaletteType {
    BACKGROUND = 0,
    SPRITE = 1
};

PaletteRam* paletteRamInit();
void paletteRamShutdown(PaletteRam* paletteRam);

uint8_t paletteRamRead(PaletteRam* paletteRam, uint16_t address);
bool paletteRamWrite(PaletteRam* paletteRam, uint16_t address, uint8_t value);

Palette paletteRamGetPalette(PaletteRam* paletteRam, uint8_t paletteNumber, enum PaletteType type);
NesPixel paletteRamGetBackgroundColor(PaletteRam* paletteRam);

#endif //ANESTOR_PALETTE_RAM_H
