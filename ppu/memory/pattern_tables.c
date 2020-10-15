#include "pattern_tables.h"

#include <stdlib.h>
#include "../../flag_ops.h"

#define BYTES_PER_TILE 16

struct pattern_tables__ {
    Cartridge* cartridge;
};



PatternTables* patternTablesInit(Cartridge* cartridge)
{
    PatternTables* patternTables = malloc( sizeof(struct pattern_tables__) );
    patternTables->cartridge = cartridge;

    return patternTables;
}



void patternTablesShutdown(PatternTables* patternTables)
{
    free(patternTables);
}



uint8_t patternTablesRead(PatternTables* patternTables, uint16_t address)
{
    return cartridgeRead(patternTables->cartridge, address);
}



bool patternTablesWrite(PatternTables* patternTables, uint16_t address, uint8_t value)
{
    return cartridgeWrite(patternTables->cartridge, address, value);
}



void patternTablesRenderTile(PatternTables* patternTables, uint8_t tile, uint8_t tableNumber, Palette palette, NesPixel* pixels)
{
    uint16_t address = (tableNumber << 12u) + (tile << 4u);
    uint8_t firstPlane[BYTES_PER_TILE / 2], secondPlane[BYTES_PER_TILE / 2];
    uint8_t pixelValue;

    uint8_t i;
    for(i=0; i < BYTES_PER_TILE / 2; i++)
        firstPlane[i] = patternTablesRead(patternTables, address + i);

    for(i=BYTES_PER_TILE / 2; i < BYTES_PER_TILE; i++)
        secondPlane[ i - BYTES_PER_TILE / 2 ] = patternTablesRead(patternTables, address + i);

    for(i=0; i < 64; i++)
    {
        pixelValue = getFlagValue(firstPlane[i / 8], 1 << (7 - (i % 8))) +
                    (getFlagValue(secondPlane[i / 8], 1 << (7 - (i % 8))) << 1u);

        pixels[i] = palette[pixelValue];
    }
}