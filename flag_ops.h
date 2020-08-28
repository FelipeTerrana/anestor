#ifndef ANESTOR_FLAG_OPS_H
#define ANESTOR_FLAG_OPS_H

#include <stdint.h>

uint8_t getFlagValue(uint8_t byte, uint8_t mask);
void setFlagValue(uint8_t* byte, uint8_t mask, uint8_t value);

#endif //ANESTOR_FLAG_OPS_H
