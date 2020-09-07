#include "header.h"

#include <stdlib.h>
#include "../flag_ops.h"

// Flag 6
#define CARTRIDGE_MIRRORING_MASK 0x01
#define CARTRIDGE_CONTAINS_PERSISTENT_MEMORY_MASK 0x02
#define CARTRIDGE_CONTAINS_TRAINER_MASK 0x04
#define CARTRIDGE_IGNORE_MIRRORING_MASK 0x08
#define CARTRIDGE_MAPPER_NUMBER_MASK 0xF0

// Flag 7
#define CARTRIDGE_VS_UNISYSTEM_MASK 0x01
#define CARTRIDGE_NES_2_0_MASK 0x0C

#define CARTRIDGE_MIRRORING_HORIZONTAL 0
#define CARTRIDGE_MIRRORING_VERTICAL 1

#define CARTRIDGE_TV_SYSTEM_NTSC 0
#define CARTRIDGE_TV_SYSTEM_PAL 1

struct cartridge_header__ {
    char nes[4];
    uint8_t prgRomSteps;
    uint8_t chrRomSteps;
    uint8_t flags6;
    uint8_t flags7;
    uint8_t prgRamSteps;
    uint8_t tvSystem;

    uint8_t padding[6];
};



CartridgeHeader* cartridgeHeaderInsert(FILE* romFile)
{
    CartridgeHeader* header = malloc( sizeof(struct cartridge_header__) );

    size_t itemsRead = fread(header, sizeof(struct cartridge_header__), 1, romFile);

    // TODO support CHR RAM
    // TODO support iNES 2.0
    if( itemsRead == 0 || header->nes[0] != 'N' || header->nes[1] != 'E' ||
        header->nes[2] != 'S' || header->nes[3] != 0x1A ||
        getFlagValue(header->flags7, CARTRIDGE_NES_2_0_MASK) == 2 || header->chrRomSteps == 0 )
    {
        free(header);
        return NULL;
    }

    return header;
}



void cartridgeHeaderRemove(CartridgeHeader* header)
{
    free(header);
}



uint16_t cartridgeHeaderGetMapper(const CartridgeHeader* header)
{
    return getFlagValue(header->flags6, CARTRIDGE_MAPPER_NUMBER_MASK) |
           (getFlagValue(header->flags7, CARTRIDGE_MAPPER_NUMBER_MASK) << 8u);
}



size_t cartridgeHeaderGetPrgRomSize(const CartridgeHeader* header)
{
    return header->prgRomSteps * CARTRIDGE_PRG_ROM_STEP;
}



size_t cartridgeHeaderGetChrRomSize(const CartridgeHeader* header)
{
    return header->chrRomSteps * CARTRIDGE_CHR_ROM_STEP;
}



bool cartridgeHeaderContainsTrainer(const CartridgeHeader* header)
{
    return getFlagValue(header->flags6, CARTRIDGE_CONTAINS_TRAINER_MASK);
}