#ifndef ANESTOR_PALETTE_H
#define ANESTOR_PALETTE_H

#include <stdint.h>

typedef struct palette__ Palette;

Palette* paletteInit(const uint8_t* paletteRamStart, const uint8_t* backgroundColor);
void paletteShutdown(Palette* palette);

uint8_t paletteGetColor(Palette* palette, uint8_t colorNumber);

#endif //ANESTOR_PALETTE_H
