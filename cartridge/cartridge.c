#include "cartridge.h"

#include <stdlib.h>
#include <string.h>
#include "mapper_controller.h"

struct cartridge__ {
    CartridgeHeader* header;
    void* mapper;
    MapperFunctions mapperFunctions;
};



Cartridge* cartridgeInsert(const char* filename)
{
    FILE* romFile = fopen(filename, "r");
    if(romFile == NULL)
    {
        printf("ROM file not found\n");
        return NULL;
    }

    Cartridge* cartridge = malloc( sizeof(struct cartridge__) );

    cartridge->header = cartridgeHeaderInsert(romFile);

    if(cartridge->header == NULL)
    {
        printf("ROM corrupted or not supported\n");
        free(cartridge);
        return NULL;
    }

    getMapperFunctions(&cartridge->mapperFunctions, cartridgeHeaderGetMapper(cartridge->header));

    if(cartridge->mapperFunctions.insert == NULL || cartridge->mapperFunctions.remove == NULL ||
       cartridge->mapperFunctions.prgRead == NULL || cartridge->mapperFunctions.prgWrite == NULL ||
       cartridge->mapperFunctions.chrRead == NULL || cartridge->mapperFunctions.chrWrite == NULL)
    {
        printf("Mapper %03d not supported\n", cartridgeHeaderGetMapper(cartridge->header));
        free(cartridge);
        return NULL;
    }

    cartridge->mapper = cartridge->mapperFunctions.insert(cartridge->header, romFile);

    if(cartridge->mapper == NULL)
    {
        cartridgeRemove(cartridge);
        return NULL;
    }

    fclose(romFile);

    printf("%s loaded successfully, mapper %03d\n", filename, cartridgeHeaderGetMapper(cartridge->header));

    return cartridge;
}



void cartridgeRemove(Cartridge* cartridge)
{
    if (cartridge != NULL)
    {
        cartridge->mapperFunctions.remove(cartridge->mapper);
        cartridgeHeaderRemove(cartridge->header);

        free(cartridge);
    }
}