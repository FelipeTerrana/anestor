#ifndef ANESTOR_PALETTE_H
#define ANESTOR_PALETTE_H

#include <stdint.h>

typedef struct {
    const uint8_t* buffer;
    const uint8_t* backgroundColor;
} Palette;

uint8_t paletteGetColor(Palette* palette, uint8_t colorNumber);

#endif //ANESTOR_PALETTE_H
