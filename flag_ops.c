#include "flag_ops.h"

uint8_t getFlagValue(uint8_t byte, uint8_t mask)
{
    if (mask == 0)
        return 0;

    while ((mask & 1u) != 1)
    {
        byte >>= 1u;
        mask >>= 1u;
    }

    return byte & mask;
}



void setFlagValue(uint8_t* byte, uint8_t mask, uint8_t value)
{
    if (mask == 0)
        return;

    *byte &= ~mask;

    uint8_t offset = 0;
    while ((mask & 1u) != 1)
    {
        mask >>= 1u;
        offset++;
    }

    mask &= value;
    mask <<= offset;

    *byte |= mask;
}