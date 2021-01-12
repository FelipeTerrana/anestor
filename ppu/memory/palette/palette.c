#include "palette.h"

#include <stdlib.h>

uint8_t paletteGetColor(Palette* palette, uint8_t colorNumber)
{
    if(colorNumber == 0)
        return *palette->backgroundColor;

    else
        return palette->buffer[colorNumber];
}