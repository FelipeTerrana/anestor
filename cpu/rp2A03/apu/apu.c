#include "apu.h"

#include <stdint.h>
#include <stdlib.h>

struct apu__ {
    uint8_t registers[APU_REGISTERS_SIZE];
};



Apu* apuInit()
{
    Apu* apu = malloc( sizeof(struct apu__) );
    return apu;
}



void apuShutdown(Apu* apu)
{
    free(apu);
}