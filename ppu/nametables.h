#ifndef ANESTOR_NAMETABLES_H
#define ANESTOR_NAMETABLES_H

#include <stdint.h>
#include "../cartridge/cartridge.h"

typedef struct nametables__ Nametables;

Nametables* nametablesInit(Cartridge* cartridge);
void nametablesShutdown(Nametables* nametables);

uint8_t nametablesRead(Nametables* nametables, uint16_t address);
bool nametablesWrite(Nametables* nametables, uint16_t address, uint8_t value);

#endif //ANESTOR_NAMETABLES_H
