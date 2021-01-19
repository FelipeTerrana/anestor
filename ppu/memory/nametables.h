#ifndef ANESTOR_NAMETABLES_H
#define ANESTOR_NAMETABLES_H

#include <stdint.h>
#include "../../cartridge/cartridge.h"

#define NUMBER_OF_NAMETABLES 4
#define TILES_PER_NAMETABLE 0x03C0

#define NAMETABLE_SPACE_START 0x2000
#define NAMETABLE_SIZE 0x0400

#define TOTAL_TILE_ROWS 60
#define TOTAL_TILE_COLS 64

typedef struct nametables__ Nametables;

Nametables* nametablesInit(Cartridge* cartridge);
void nametablesShutdown(Nametables* nametables);

uint8_t nametablesRead(Nametables* nametables, uint16_t address);
bool nametablesWrite(Nametables* nametables, uint16_t address, uint8_t value);

uint8_t nametablesGetTileIndex(Nametables* nametables, uint8_t offsetX, uint8_t offsetY);
uint8_t nametablesGetTilePaletteNumber(Nametables* nametables, uint8_t offsetX, uint8_t offsetY);

#endif //ANESTOR_NAMETABLES_H
