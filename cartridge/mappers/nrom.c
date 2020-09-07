#include "nrom.h"

#include <stdlib.h>

struct nrom__ {
    uint8_t* trainer;
    uint8_t* prgRom;
    uint8_t* chrRom;
};


void* nromInsert(const CartridgeHeader* header, FILE* romFile)
{
    Nrom* nrom = malloc( sizeof(struct nrom__) );
    size_t itemsRead;

    nrom->prgRom = malloc( cartridgeHeaderGetPrgRomSize(header) );
    nrom->chrRom = malloc( cartridgeHeaderGetChrRomSize(header) );

    if( cartridgeHeaderContainsTrainer(header) )
    {
        nrom->trainer = malloc(CARTRIDGE_TRAINER_MAX_SIZE);

        itemsRead = fread(nrom->trainer, 1, CARTRIDGE_TRAINER_MAX_SIZE, romFile);

        if(itemsRead != CARTRIDGE_TRAINER_MAX_SIZE)
        {
            printf("Trainer bytes expected: 0x%04X, received 0x%04zX\n", CARTRIDGE_TRAINER_MAX_SIZE, itemsRead);
            nromRemove(nrom);
            return NULL;
        }
    }
    else
        nrom->trainer = NULL;

    itemsRead = fread(nrom->prgRom, 1, cartridgeHeaderGetPrgRomSize(header), romFile);

    if(itemsRead != cartridgeHeaderGetPrgRomSize(header))
    {
        printf("PRG ROM bytes expected: 0x%04zX, received 0x%04zX\n", cartridgeHeaderGetPrgRomSize(header), itemsRead);
        nromRemove(nrom);
        return NULL;
    }

    itemsRead = fread(nrom->chrRom, 1, cartridgeHeaderGetChrRomSize(header), romFile);

    if(itemsRead != cartridgeHeaderGetChrRomSize(header))
    {
        printf("CHR ROM bytes expected: 0x%04zX, received 0x%04zX\n", cartridgeHeaderGetChrRomSize(header), itemsRead);
        nromRemove(nrom);
        return NULL;
    }

    return nrom;
}



void nromRemove(void* mapper)
{
    Nrom* nrom = mapper;

    if(nrom != NULL)
    {
        free(nrom->trainer);
        free(nrom->prgRom);
        free(nrom->chrRom);

        free(nrom);
    }
}



uint8_t nromRead(void* mapper, uint16_t address)
{
    // TODO
    return 0x0;
}



bool nromWrite(void* mapper, uint16_t address, uint8_t value)
{
    // TODO
    return false;
}