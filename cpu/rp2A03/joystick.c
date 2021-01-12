#include "joystick.h"

#include <stdlib.h>
#include "../../flag_ops.h"

enum JoystickButton {
    JOY_A = 0,
    JOY_B = 1,
    JOY_SELECT = 2,
    JOY_START = 3,
    JOY_UP = 4,
    JOY_DOWN = 5,
    JOY_LEFT = 6,
    JOY_RIGHT = 7,

    JOY_END_OF_LOOP = 8
};

enum Player {
    PLAYER_1 = 0,
    PLAYER_2 = 1
};



struct joystick__ {
    uint8_t strobe;
    enum JoystickButton nextToRead;
};



static uint8_t buttonStatus(Joystick* joystick, enum JoystickButton button, enum Player player)
{
    return 0x0;
}



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
    *value = 0x40;

    if(joystick->strobe == 0 && joystick->nextToRead < JOY_END_OF_LOOP)
        *value |= buttonStatus(joystick, joystick->nextToRead++, address % 2);

    else if(joystick->strobe == 0 && joystick->nextToRead >= JOY_END_OF_LOOP)
        *value |= 0x01;

    else if(joystick->strobe == 1)
        *value |= buttonStatus(joystick, JOY_A, address % 2);

    return 0;
}



uint16_t joystickWrite(Joystick* joystick, uint16_t address, uint8_t value)
{
    if(getFlagValue(joystick->strobe, 0x01) == 1 && getFlagValue(value, 0x01) == 0)
        joystick->nextToRead = JOY_A;

    joystick->strobe = value;
    return 0;
}