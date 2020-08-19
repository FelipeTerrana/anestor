#include "cpu.h"

#include <stdint.h>
#include "instructions.h"

struct cpu__ {
    CpuRegisters* registers;
    CpuMemory* memory;
};

static enum AddressingMode getAddressingMode__(uint8_t instruction)
{
    static const enum AddressingMode INSTRUCTION_ADDRESSING_EXCEPTIONS[0x100] = {
            [0x96] = ADDRESSING_ZERO_PAGE_Y,
            [0xB6] = ADDRESSING_ZERO_PAGE_Y,
            [0xBE] = ADDRESSING_ABSOLUTE_Y,
            [0x10] = ADDRESSING_RELATIVE,
            [0x30] = ADDRESSING_RELATIVE,
            [0x50] = ADDRESSING_RELATIVE,
            [0x70] = ADDRESSING_RELATIVE,
            [0x90] = ADDRESSING_RELATIVE,
            [0xB0] = ADDRESSING_RELATIVE,
            [0xD0] = ADDRESSING_RELATIVE,
            [0xF0] = ADDRESSING_RELATIVE,
            [0x00] = ADDRESSING_IMPLIED,
            [0x20] = ADDRESSING_ABSOLUTE,
            [0x40] = ADDRESSING_IMPLIED,
            [0x60] = ADDRESSING_IMPLIED,
            [0x08] = ADDRESSING_IMPLIED,
            [0x28] = ADDRESSING_IMPLIED,
            [0x48] = ADDRESSING_IMPLIED,
            [0x68] = ADDRESSING_IMPLIED,
            [0x88] = ADDRESSING_IMPLIED,
            [0xA8] = ADDRESSING_IMPLIED,
            [0xC8] = ADDRESSING_IMPLIED,
            [0xE8] = ADDRESSING_IMPLIED,
            [0x18] = ADDRESSING_IMPLIED,
            [0x38] = ADDRESSING_IMPLIED,
            [0x58] = ADDRESSING_IMPLIED,
            [0x78] = ADDRESSING_IMPLIED,
            [0x98] = ADDRESSING_IMPLIED,
            [0xB8] = ADDRESSING_IMPLIED,
            [0xD8] = ADDRESSING_IMPLIED,
            [0xF8] = ADDRESSING_IMPLIED,
            [0x8A] = ADDRESSING_IMPLIED,
            [0x9A] = ADDRESSING_IMPLIED,
            [0xAA] = ADDRESSING_IMPLIED,
            [0xBA] = ADDRESSING_IMPLIED,
            [0xCA] = ADDRESSING_IMPLIED,
            [0xEA] = ADDRESSING_IMPLIED,

            [0xFF] = ADDRESSING_INVALID
    };

    static const enum AddressingMode INSTRUCTION_ADDRESSING_CC_BBB[0x4][0x8] = {
            [0x1] = {
                    [0x0] = ADDRESSING_INDIRECT_X,
                    [0x1] = ADDRESSING_ZERO_PAGE,
                    [0x2] = ADDRESSING_IMMEDIATE,
                    [0x3] = ADDRESSING_ABSOLUTE,
                    [0x4] = ADDRESSING_INDIRECT_Y,
                    [0x5] = ADDRESSING_ZERO_PAGE_X,
                    [0x6] = ADDRESSING_ABSOLUTE_Y,
                    [0x7] = ADDRESSING_ABSOLUTE_X
            },
            [0x2] = {
                    [0x0] = ADDRESSING_IMMEDIATE,
                    [0x1] = ADDRESSING_ZERO_PAGE,
                    [0x2] = ADDRESSING_ACCUMULATOR,
                    [0x3] = ADDRESSING_ABSOLUTE,
                    [0x5] = ADDRESSING_ZERO_PAGE_X,
                    [0x7] = ADDRESSING_ABSOLUTE_X,

                    [0x4] = ADDRESSING_INVALID,
                    [0x6] = ADDRESSING_INVALID
            },
            [0x0] = {
                    [0x0] = ADDRESSING_IMMEDIATE,
                    [0x1] = ADDRESSING_ZERO_PAGE,
                    [0x3] = ADDRESSING_ABSOLUTE,
                    [0x5] = ADDRESSING_ZERO_PAGE_X,
                    [0x7] = ADDRESSING_ABSOLUTE_X,

                    [0x2] = ADDRESSING_INVALID,
                    [0x4] = ADDRESSING_INVALID,
                    [0x6] = ADDRESSING_INVALID
            },

            [0x3] = { ADDRESSING_INVALID }
    };

    uint8_t bbb, cc;
    uint8_t addressing = INSTRUCTION_ADDRESSING_EXCEPTIONS[instruction];

    if(addressing != ADDRESSING_INVALID)
        return addressing;

    cc = instruction & 0x3u;
    instruction >>= 2u;

    bbb = instruction & 0x7u;

    return INSTRUCTION_ADDRESSING_CC_BBB[cc][bbb];
}