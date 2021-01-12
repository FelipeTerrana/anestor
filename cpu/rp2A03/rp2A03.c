#include "rp2A03.h"

#include <stdlib.h>
#include "apu/apu.h"
#include "joystick.h"

#define JOYSTICK_1_READ_ADDRESS 0x4016
#define JOYSTICK_2_READ_ADDRESS 0x4017

#define JOYSTICK_WRITE_ADDRESS 0x4016

struct rp2A03__ {
    Apu* apu;
    Joystick* joystick;
};

Rp2A03* rp2A03Init()
{
    Rp2A03* rp2A03 = malloc( sizeof(struct rp2A03__) );
    rp2A03->apu = apuInit();
    rp2A03->joystick = joystickInit();

    return rp2A03;
}



void rp2A03Shutdown(Rp2A03* rp2A03)
{
    apuShutdown(rp2A03->apu);
    joystickShutdown(rp2A03->joystick);

    free(rp2A03);
}



uint16_t rp2A03Read(Rp2A03* rp2A03, uint16_t address, uint8_t* value)
{
    if(address == JOYSTICK_1_READ_ADDRESS || address == JOYSTICK_2_READ_ADDRESS)
        return joystickRead(rp2A03->joystick, address, value);

    else
        return apuRead(rp2A03->apu, address, value);
}



uint16_t rp2A03Write(Rp2A03* rp2A03, uint16_t address, uint8_t value)
{
    if(address == JOYSTICK_WRITE_ADDRESS)
        return joystickWrite(rp2A03->joystick, address, value);

    else
        return apuWrite(rp2A03->apu, address, value);
}