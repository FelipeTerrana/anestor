#include "cartridge.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "flag_ops.h"

#define CARTRIDGE_TRAINER_MAX_SIZE 512
#define CARTRIDGE_PRG_ROM_STEP 16384
#define CARTRIDGE_CHR_ROM_STEP 8192
#define CARTRIDGE_CHR_RAM_STEP 8192
#define CARTRIDGE_TITLE_SIZE 128

// Flag 6
#define CARTRIDGE_MIRRORING_MASK 0x01
#define CARTRIDGE_CONTAINS_PERSISTENT_MEMORY_MASK 0x02
#define CARTRIDGE_CONTAINS_TRAINER_MASK 0x04
#define CARTRIDGE_IGNORE_MIRRORING_MASK 0x08
#define CARTRIDGE_MAPPER_NUMBER_MASK 0xF0

// Flag 7
#define CARTRIDGE_VS_UNISYSTEM_MASK 0x01
#define CARTRIDGE_NES_2_0_MASK 0x0C

#define CARTRIDGE_MIRRORING_HORIZONTAL = 0
#define CARTRIDGE_MIRRORING_VERTICAL = 1

#define CARTRIDGE_TV_SYSTEM_NTSC 0
#define CARTRIDGE_TV_SYSTEM_PAL 1

struct header__ {
    char nes[4];
    uint8_t prgRomSteps;
    uint8_t chrRomSteps;
    uint8_t flags6;
    uint8_t flags7;
    uint8_t prgRamSteps;
    uint8_t tvSystem;

    uint8_t padding[6];
};

struct cartridge__ {
    struct header__ header;
    uint8_t* trainer;
    uint8_t* prgRom;
    uint8_t* chrRom;
    char title[CARTRIDGE_TITLE_SIZE + 1];
};



static uint8_t cartridgeGetMapper__(Cartridge* cartridge)
{
    return getFlagValue(cartridge->header.flags6, CARTRIDGE_MAPPER_NUMBER_MASK) |
           (getFlagValue(cartridge->header.flags7, CARTRIDGE_MAPPER_NUMBER_MASK) << 8u);
}



Cartridge* cartridgeInsert(const char* filename)
{
    FILE* romFile = fopen(filename, "r");
    if(romFile == NULL)
    {
        printf("ROM file not found\n");
        return NULL;
    }

    Cartridge* cartridge = malloc( sizeof(struct cartridge__) );

    size_t itemsRead = fread(&cartridge->header, sizeof(struct header__), 1, romFile);

    // TODO support CHR RAM
    // TODO support iNES 2.0
    if( itemsRead == 0 || cartridge->header.nes[0] != 'N' || cartridge->header.nes[1] != 'E' ||
        cartridge->header.nes[2] != 'S' || cartridge->header.nes[3] != 0x1A ||
            getFlagValue(cartridge->header.flags7, CARTRIDGE_NES_2_0_MASK) == 2 ||
        cartridge->header.chrRomSteps == 0 )
    {
        printf("ROM corrupted or not supported\n");
        free(cartridge);
        return NULL;
    }

    cartridge->prgRom = malloc( cartridge->header.prgRomSteps * CARTRIDGE_PRG_ROM_STEP );
    cartridge->chrRom = malloc( cartridge->header.chrRomSteps * CARTRIDGE_CHR_ROM_STEP );

    if(getFlagValue(cartridge->header.flags6, CARTRIDGE_CONTAINS_TRAINER_MASK) )
    {
        cartridge->trainer = malloc(CARTRIDGE_TRAINER_MAX_SIZE);

        itemsRead = fread(cartridge->trainer, 1, CARTRIDGE_TRAINER_MAX_SIZE, romFile);

        if(itemsRead != CARTRIDGE_TRAINER_MAX_SIZE)
        {
            printf("Trainer bytes expected: 0x%04X, received 0x%04zX\n", CARTRIDGE_TRAINER_MAX_SIZE, itemsRead);
            cartridgeRemove(cartridge);
            return NULL;
        }
    }
    else
        cartridge->trainer = NULL;

    itemsRead = fread(cartridge->prgRom, 1, cartridge->header.prgRomSteps * CARTRIDGE_PRG_ROM_STEP, romFile);

    if(itemsRead != cartridge->header.prgRomSteps * CARTRIDGE_PRG_ROM_STEP)
    {
        printf("PRG ROM bytes expected: 0x%04X, received 0x%04zX\n", cartridge->header.prgRomSteps * CARTRIDGE_PRG_ROM_STEP, itemsRead);
        cartridgeRemove(cartridge);
        return NULL;
    }

    itemsRead = fread(cartridge->chrRom, 1, cartridge->header.chrRomSteps * CARTRIDGE_CHR_ROM_STEP, romFile);

    if(itemsRead != cartridge->header.chrRomSteps * CARTRIDGE_CHR_ROM_STEP)
    {
        printf("CHR ROM bytes expected: 0x%04X, received 0x%04zX\n", cartridge->header.chrRomSteps * CARTRIDGE_CHR_ROM_STEP, itemsRead);
        cartridgeRemove(cartridge);
        return NULL;
    }

    fgets(cartridge->title, CARTRIDGE_TITLE_SIZE + 1, romFile);
    fclose(romFile);

    printf("%s loaded successfully (%s, mapper %03d)\n",
           filename,
           strlen(cartridge->title) > 0 ? cartridge->title : "No title",
           cartridgeGetMapper__(cartridge));

    return cartridge;
}



void cartridgeRemove(Cartridge* cartridge)
{
    if (cartridge != NULL)
    {
        free(cartridge->prgRom);
        free(cartridge->chrRom);
        free(cartridge->trainer);

        free(cartridge);
    }
}