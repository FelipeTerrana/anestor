#include "apu.h"

#include <stdint.h>
#include <stdlib.h>

struct apu__ {

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



uint16_t apuRead(Apu* apu, uint16_t address, uint8_t* value)
{
    // TODO APU read
    return 0;
}



uint16_t apuWrite(Apu* apu, uint16_t address, uint8_t value)
{
    // TODO APU write
    return 0;
}