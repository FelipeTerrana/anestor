#include "nrom.h"

#include <stdlib.h>

#define CHR_SPACE_FIRST_ADDRESS 0x0000
#define CHR_SPACE_LAST_ADDRESS  0x1FFF

#define PRG_SPACE_FIRST_ADDRESS 0x8000

struct nrom__ {
    const CartridgeHeader* header;
    uint8_t* trainer;
    uint8_t* prgRom;
    uint8_t* chrRom;
};


void* nromInsert(const CartridgeHeader* header, FILE* romFile)
{
    if( cartridgeHeaderGetPrgRomSize(header) == 0 || cartridgeHeaderGetChrRomSize(header) == 0 )
    {
        printf("PRG ROM or CHR ROM empty\n");
        return NULL;
    }

    Nrom* nrom = malloc( sizeof(struct nrom__) );
    size_t itemsRead;

    nrom->header = header;
    nrom->prgRom = malloc( cartridgeHeaderGetPrgRomSize(nrom->header) );
    nrom->chrRom = malloc( cartridgeHeaderGetChrRomSize(nrom->header) );

    if( cartridgeHeaderContainsTrainer(nrom->header) )
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

    itemsRead = fread(nrom->prgRom, 1, cartridgeHeaderGetPrgRomSize(nrom->header), romFile);

    if(itemsRead != cartridgeHeaderGetPrgRomSize(nrom->header))
    {
        printf("PRG ROM bytes expected: 0x%04zX, received 0x%04zX\n", cartridgeHeaderGetPrgRomSize(nrom->header), itemsRead);
        nromRemove(nrom);
        return NULL;
    }

    itemsRead = fread(nrom->chrRom, 1, cartridgeHeaderGetChrRomSize(nrom->header), romFile);

    if(itemsRead != cartridgeHeaderGetChrRomSize(nrom->header))
    {
        printf("CHR ROM bytes expected: 0x%04zX, received 0x%04zX\n", cartridgeHeaderGetChrRomSize(nrom->header), itemsRead);
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
    Nrom* nrom = mapper;

    if(address >= CHR_SPACE_FIRST_ADDRESS && address <= CHR_SPACE_LAST_ADDRESS)
        return nrom->chrRom[address % cartridgeHeaderGetChrRomSize(nrom->header)];

    else if(address >= PRG_SPACE_FIRST_ADDRESS)
        return nrom->prgRom[address % cartridgeHeaderGetPrgRomSize(nrom->header)];

    else
        return 0x0;
}



bool nromWrite(void* mapper, uint16_t address, uint8_t value)
{
    // NROM (except for Family BASIC) doesn't have writable RAM
    return false;
}