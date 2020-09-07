#ifndef ANESTOR_CARTRIDGE_H
#define ANESTOR_CARTRIDGE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "header.h"

typedef struct cartridge__ Cartridge;



typedef struct {
    void* (*insert) (const CartridgeHeader* header, FILE* romFile);
    void (*remove) (void* mapper);

    uint8_t (*prgRead) (void* mapper, uint16_t address);
    bool (*prgWrite) (void* mapper, uint16_t address, uint8_t value);

    uint8_t (*chrRead) (void* mapper, uint16_t address);
    bool (*chrWrite) (void* mapper, uint16_t address, uint8_t value);
} MapperInfo;



Cartridge* cartridgeInsert(const char* filename);
void cartridgeRemove(Cartridge* cartridge);

#endif //ANESTOR_CARTRIDGE_H
