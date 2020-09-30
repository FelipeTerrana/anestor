#ifndef ANESTOR_NAMETABLES_H
#define ANESTOR_NAMETABLES_H

#include <stdint.h>
#include "../../cartridge/cartridge.h"

#define NUMBER_OF_NAMETABLES 4
#define TILES_PER_NAMETABLE 0x03C0

#define NAMETABLE_SPACE_START 0x2000
#define NAMETABLE_SIZE 0x0400

typedef struct nametables__ Nametables;

Nametables* nametablesInit(Cartridge* cartridge);
void nametablesShutdown(Nametables* nametables);

uint8_t nametablesRead(Nametables* nametables, uint16_t address);
bool nametablesWrite(Nametables* nametables, uint16_t address, uint8_t value);

#endif //ANESTOR_NAMETABLES_H
