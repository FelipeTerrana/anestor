#include "joystick.h"

#include <stdlib.h>



struct joystick__ {

};



Joystick* joystickInit()
{
    Joystick* joystick = malloc( sizeof(struct joystick__) );
    return joystick;
}



void joystickShutdown(Joystick* joystick)
{
    free(joystick);
}



uint16_t joystickRead(Joystick* joystick, uint16_t address, uint8_t* value)
{
    // TODO joystickRead
    return 0;
}



uint16_t joystickWrite(Joystick* joystick, uint16_t address, uint8_t value)
{
    // TODO joystickWrite
    return 0;
}