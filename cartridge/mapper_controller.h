#ifndef ANESTOR_MAPPER_CONTROLLER_H
#define ANESTOR_MAPPER_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>
#include "header.h"

typedef struct {
    void* (*insert) (const CartridgeHeader* header, FILE* romFile);
    void (*remove) (void* mapper);

    uint8_t (*prgRead) (void* mapper, uint16_t address);
    bool (*prgWrite) (void* mapper, uint16_t address, uint8_t value);

    uint8_t (*chrRead) (void* mapper, uint16_t address);
    bool (*chrWrite) (void* mapper, uint16_t address, uint8_t value);
} MapperFunctions;



void getMapperFunctions(MapperFunctions* functions, uint16_t mapperNumber);

#endif //ANESTOR_MAPPER_CONTROLLER_H
