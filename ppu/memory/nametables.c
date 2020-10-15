#include "nametables.h"

#include <stdlib.h>
#include <string.h>

#define VRAM_SIZE 0x0800

struct nametables__ {
    uint8_t vram[VRAM_SIZE];
    Cartridge* cartridge;
    enum NametableMirroring mirroring;
};



Nametables* nametablesInit(Cartridge* cartridge)
{
    Nametables* nametables = malloc( sizeof(struct nametables__) );
    nametables->cartridge = cartridge;
    nametables->mirroring = cartridgeGetMirroring(nametables->cartridge);

    memset(nametables->vram, 0, VRAM_SIZE);

    return nametables;
}



void nametablesShutdown(Nametables* nametables)
{
    free(nametables);
}



static uint16_t getVramIndex__(uint16_t address, enum NametableMirroring mirroring)
{
    if(mirroring == HORIZONTAL)
        return (address - NAMETABLE_SPACE_START) % NAMETABLE_SIZE +
                ((address >= (NAMETABLE_SPACE_START + 2 * NAMETABLE_SIZE)) ? (VRAM_SIZE / 2) : 0);

    else if(mirroring == VERTICAL)
        return (address - NAMETABLE_SPACE_START) % (2 * NAMETABLE_SIZE);

    return -1;
}



uint8_t nametablesRead(Nametables* nametables, uint16_t address)
{
    if( nametables->mirroring == FOUR_SCREEN )
        return cartridgeRead(nametables->cartridge, address);

    else
        return nametables->vram[ getVramIndex__(address, nametables->mirroring) ];
}



bool nametablesWrite(Nametables* nametables, uint16_t address, uint8_t value)
{
    if( nametables->mirroring == FOUR_SCREEN )
        return cartridgeWrite(nametables->cartridge, address, value);

    else
    {
        nametables->vram[ getVramIndex__(address, nametables->mirroring) ] = value;
        return true;
    }
}