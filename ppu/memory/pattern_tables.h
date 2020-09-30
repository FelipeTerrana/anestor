#ifndef ANESTOR_PATTERN_TABLES_H
#define ANESTOR_PATTERN_TABLES_H

#include <stdint.h>
#include <stdbool.h>
#include "../../cartridge/cartridge.h"

typedef struct pattern_tables__ PatternTables;

PatternTables* patternTablesInit(Cartridge* cartridge);
void patternTablesShutdown(PatternTables* patternTables);

uint8_t patternTablesRead(PatternTables* patternTables, uint16_t address);
bool patternTablesWrite(PatternTables* patternTables, uint16_t address, uint8_t value);

#endif //ANESTOR_PATTERN_TABLES_H
