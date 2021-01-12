#include "palette.h"

#include <stdlib.h>

struct palette__ {
    const uint8_t* buffer;
    const uint8_t* backgroundColor;
};

Palette* paletteInit(const uint8_t* paletteRamStart, const uint8_t* backgroundColor)
{
    Palette* palette = malloc( sizeof(struct palette__) );
    palette->buffer = paletteRamStart;
    palette->backgroundColor = backgroundColor;

    return palette;
}



void paletteShutdown(Palette* palette)
{
    free(palette);
}



uint8_t paletteGetColor(Palette* palette, uint8_t colorNumber)
{
    if(colorNumber == 0)
        return *palette->backgroundColor;

    else
        return palette->buffer[colorNumber];
}