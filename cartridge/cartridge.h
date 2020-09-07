#ifndef ANESTOR_CARTRIDGE_H
#define ANESTOR_CARTRIDGE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "header.h"

typedef struct cartridge__ Cartridge;

Cartridge* cartridgeInsert(const char* filename);
void cartridgeRemove(Cartridge* cartridge);

uint8_t cartridgeRead(Cartridge* cartridge, uint16_t address);
bool cartridgeWrite(Cartridge* cartridge, uint16_t address, uint8_t value);

#endif //ANESTOR_CARTRIDGE_H
