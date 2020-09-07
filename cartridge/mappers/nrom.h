#ifndef ANESTOR_NROM_H
#define ANESTOR_NROM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../header.h"

typedef struct nrom__ Nrom;

void* nromInsert(const CartridgeHeader* header, FILE* romFile);
void nromRemove(void* mapper);

uint8_t nromPrgRead(void* mapper, uint16_t address);
bool nromPrgWrite(void* mapper, uint16_t address, uint8_t value);

uint8_t nromChrRead(void* mapper, uint16_t address);
bool nromChrWrite(void* mapper, uint16_t address, uint8_t value);

#endif //ANESTOR_NROM_H
