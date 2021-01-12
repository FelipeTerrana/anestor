#ifndef ANESTOR_PATTERN_TABLES_H
#define ANESTOR_PATTERN_TABLES_H

#include <stdint.h>
#include <stdbool.h>
#include "../../cartridge/cartridge.h"
#include "../screen.h"
#include "palette/palette_ram.h"
#include "palette/palette.h"

typedef struct pattern_tables__ PatternTables;

PatternTables* patternTablesInit(Cartridge* cartridge);
void patternTablesShutdown(PatternTables* patternTables);

uint8_t patternTablesRead(PatternTables* patternTables, uint16_t address);
bool patternTablesWrite(PatternTables* patternTables, uint16_t address, uint8_t value);

void patternTablesRenderTile(PatternTables* patternTables, uint8_t tile, uint8_t tableNumber, Palette* palette, NesPixel pixels[]);

#endif //ANESTOR_PATTERN_TABLES_H
