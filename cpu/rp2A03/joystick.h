#ifndef ANESTOR_JOYSTICK_H
#define ANESTOR_JOYSTICK_H

#include <stdint.h>

typedef struct joystick__ Joystick;

Joystick* joystickInit();
void joystickShutdown(Joystick* joystick);

uint16_t joystickRead(Joystick* joystick, uint16_t address, uint8_t* value);
uint16_t joystickWrite(Joystick* joystick, uint16_t address, uint8_t value);

#endif //ANESTOR_JOYSTICK_H
