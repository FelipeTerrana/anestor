#include "nrom.h"

#include <stdlib.h>
#include <stdbool.h>

#define CHR_SPACE_FIRST_ADDRESS 0x0000
#define CHR_SPACE_LAST_ADDRESS  0x1FFF

#define PRG_SPACE_FIRST_ADDRESS 0x8000

#define CHR_RAM_SIZE 0x2000

struct nrom__ {
    const CartridgeHeader* header;
    uint8_t* trainer;
    uint8_t* prgRom;
    uint8_t* chrRom;
    bool chrRam;
    size_t chrSize;
};


void* nromInsert(const CartridgeHeader* header, FILE* romFile)
{
    if( cartridgeHeaderGetPrgRomSize(header) == 0 )
    {
        printf("PRG ROM empty\n");
        return NULL;
    }

    Nrom* nrom = malloc( sizeof(struct nrom__) );
    size_t itemsRead;

    if( cartridgeHeaderGetChrRomSize(header) == 0 )
    {
        nrom->chrRam = true;
        nrom->chrSize = CHR_RAM_SIZE;
    }
    else
    {
        nrom->chrRam = false;
        nrom->chrSize = cartridgeHeaderGetChrRomSize(header);
    }

    nrom->header = header;
    nrom->prgRom = malloc( cartridgeHeaderGetPrgRomSize(nrom->header) );
    nrom->chrRom = malloc(nrom->chrSize);

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

    if(!nrom->chrRam)
    {
        itemsRead = fread(nrom->chrRom, 1, nrom->chrSize, romFile);

        if (itemsRead != nrom->chrSize)
        {
            printf("CHR ROM bytes expected: 0x%04zX, received 0x%04zX\n", nrom->chrSize, itemsRead);
            nromRemove(nrom);
            return NULL;
        }
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
        return nrom->chrRom[address % nrom->chrSize];

    else if(address >= PRG_SPACE_FIRST_ADDRESS)
        return nrom->prgRom[address % cartridgeHeaderGetPrgRomSize(nrom->header)];

    else
        return 0x0;
}



bool nromWrite(void* mapper, uint16_t address, uint8_t value)
{
    Nrom* nrom = mapper;

    if(address >= CHR_SPACE_FIRST_ADDRESS && address <= CHR_SPACE_LAST_ADDRESS && nrom->chrRam)
    {
        nrom->chrRom[address % nrom->chrSize] = value;
        return true;
    }

    return false;
}