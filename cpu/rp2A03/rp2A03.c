#include "rp2A03.h"

#include <stdlib.h>
#include "apu/apu.h"

struct rp2A03__ {
    PpuMemory* ppuMemory;
    Apu* apu;
};

Rp2A03* rp2A03Init(PpuMemory* ppuMemory)
{
    Rp2A03* rp2A03 = malloc( sizeof(struct rp2A03__) );
    rp2A03->ppuMemory = ppuMemory;
    rp2A03->apu = apuInit();

    return rp2A03;
}



void rp2A03Shutdown(Rp2A03* rp2A03)
{
    apuShutdown(rp2A03->apu);
    free(rp2A03);
}



uint16_t rp2A03Read(Rp2A03* rp2A03, uint16_t address, uint8_t* value)
{
    // TODO RP2A03 read
}



uint16_t rp2A03Write(Rp2A03* rp2A03, uint16_t address, uint8_t value)
{
    // TODO RP2A03 write
}