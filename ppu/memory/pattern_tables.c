#include "pattern_tables.h"

#include <stdlib.h>

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