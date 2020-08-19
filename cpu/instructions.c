#include "instructions.h"

#define CPU_RAM_STACK_START 0x0100

struct cpu_registers__ {
    uint8_t acc;
    uint8_t x, y;
    uint16_t pc;
    uint8_t s;
    uint8_t p;
};

const uint8_t ADRESSING_EXTRA_BYTES[NUM_ADDRESSING_MODES] = {
        [ADDRESSING_IMPLIED] = 0,
        [ADDRESSING_ACCUMULATOR] = 0,
        [ADDRESSING_IMMEDIATE] = 1,
        [ADDRESSING_ZERO_PAGE] = 1,
        [ADDRESSING_ZERO_PAGE_X] = 1,
        [ADDRESSING_ZERO_PAGE_Y] = 1,
        [ADDRESSING_RELATIVE] = 1,
        [ADDRESSING_ABSOLUTE] = 2,
        [ADDRESSING_ABSOLUTE_X] = 2,
        [ADDRESSING_ABSOLUTE_Y] = 2,
        [ADDRESSING_INDIRECT] = 2,
        [ADDRESSING_INDIRECT_X] = 1,
        [ADDRESSING_INDIRECT_Y] = 1,

        [ADDRESSING_INVALID] = -1
};