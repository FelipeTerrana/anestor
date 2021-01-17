#include "mapper_controller.h"

#include "mappers/nrom.h"

#define MAPPER_ARRAY_SIZE 559

static const MapperFunctions NROM = {
        nromInsert,
        nromRemove,
        nromRead,
        nromWrite,
};



static const MapperFunctions MAPPER_ARRAY[MAPPER_ARRAY_SIZE] = {
        [0] = NROM,
        [1] = NROM, // TODO implement MMC1
        [3] = NROM, // TODO implement CNROM
        [MAPPER_ARRAY_SIZE - 1] = {NULL, NULL, NULL, NULL}
};



void getMapperFunctions(MapperFunctions* functions, uint16_t mapperNumber)
{
    *functions = MAPPER_ARRAY[mapperNumber];
}