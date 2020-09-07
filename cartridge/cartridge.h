#ifndef ANESTOR_CARTRIDGE_H
#define ANESTOR_CARTRIDGE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "header.h"

typedef struct cartridge__ Cartridge;

Cartridge* cartridgeInsert(const char* filename);
void cartridgeRemove(Cartridge* cartridge);

#endif //ANESTOR_CARTRIDGE_H
