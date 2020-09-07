#include "cartridge.h"

#include <stdlib.h>
#include <string.h>
#include "../flag_ops.h"

struct cartridge__ {
    CartridgeHeader* header;
    void* mapper;
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

    // TODO init mapper

    fclose(romFile);

    printf("%s loaded successfully, mapper %03d\n", filename, cartridgeHeaderGetMapper(cartridge->header));

    return cartridge;
}



void cartridgeRemove(Cartridge* cartridge)
{
    if (cartridge != NULL)
    {
        cartridgeHeaderRemove(cartridge->header);
        // TODO remove mapper

        free(cartridge);
    }
}