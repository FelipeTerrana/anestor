#ifndef ANESTOR_MAPPER_CONTROLLER_H
#define ANESTOR_MAPPER_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>
#include "header.h"

typedef struct {
    void* (*insert) (const CartridgeHeader* header, FILE* romFile);
    void (*remove) (void* mapper);

    uint8_t (*read) (void* mapper, uint16_t address);
    bool (*write) (void* mapper, uint16_t address, uint8_t value);
} MapperFunctions;



void getMapperFunctions(MapperFunctions* functions, uint16_t mapperNumber);

#endif //ANESTOR_MAPPER_CONTROLLER_H
