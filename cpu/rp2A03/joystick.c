#include "joystick.h"

#include <SDL.h>
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



static SDL_Scancode KEY_BINDINGS[2][8] = {
        [PLAYER_1] = {
                [JOY_A] = SDL_SCANCODE_KP_2,
                [JOY_B] = SDL_SCANCODE_KP_1,
                [JOY_SELECT] = SDL_SCANCODE_SPACE,
                [JOY_START] = SDL_SCANCODE_RETURN,
                [JOY_UP] = SDL_SCANCODE_W,
                [JOY_DOWN] = SDL_SCANCODE_S,
                [JOY_LEFT] = SDL_SCANCODE_A,
                [JOY_RIGHT] = SDL_SCANCODE_D,
        },

        [PLAYER_2] = {
                [JOY_A] = SDL_SCANCODE_UNKNOWN,
                [JOY_B] = SDL_SCANCODE_UNKNOWN,
                [JOY_SELECT] = SDL_SCANCODE_UNKNOWN,
                [JOY_START] = SDL_SCANCODE_UNKNOWN,
                [JOY_UP] = SDL_SCANCODE_UNKNOWN,
                [JOY_DOWN] = SDL_SCANCODE_UNKNOWN,
                [JOY_LEFT] = SDL_SCANCODE_UNKNOWN,
                [JOY_RIGHT] = SDL_SCANCODE_UNKNOWN,
        }
};



struct joystick__ {
    uint8_t strobe;
    enum JoystickButton nextToRead;
};



static uint8_t buttonStatus(enum JoystickButton button, enum Player player)
{
    return SDL_GetKeyboardState(NULL)[ KEY_BINDINGS[player][button] ];
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
        *value |= buttonStatus(joystick->nextToRead++, address % 2);

    else if(joystick->strobe == 0 && joystick->nextToRead >= JOY_END_OF_LOOP)
        *value |= 0x01;

    else if(joystick->strobe == 1)
        *value |= buttonStatus(JOY_A, address % 2);

    return 0;
}



uint16_t joystickWrite(Joystick* joystick, uint16_t address, uint8_t value)
{
    if(getFlagValue(joystick->strobe, 0x01) == 1 && getFlagValue(value, 0x01) == 0)
        joystick->nextToRead = JOY_A;

    joystick->strobe = value;
    return 0;
}