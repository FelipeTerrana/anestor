#include "cpu.h"

#include <stdint.h>
#include "cartridge.h"



#define CPU_RAM_SIZE 0x0800

#define PPU_REGISTERS_START 0x2000
#define PPU_REGISTERS_SIZE 0x0008

#define APU_IO_REGISTERS_START 0x4000
#define APU_IO_TEST_REGISTERS_START 0x4018
#define CPU_CARTRIDGE_SPACE_START 0x4020

#define CPU_RAM_STACK_START 0x0100

struct cpu_registers__ {
    uint8_t acc;
    uint8_t x, y;
    uint16_t pc;
    uint8_t s;
    uint8_t p;
};

struct cpu__ {
    struct cpu_registers__ registers;
    uint8_t ram[CPU_RAM_SIZE];
    // TODO ponteiro pra PPU
    // TODO ponteiro pra APU
    Cartridge* cartridge;
};



enum AddressingMode
{
    ADDRESSING_IMPLIED = 1,
    ADDRESSING_ACCUMULATOR = 2,
    ADDRESSING_IMMEDIATE = 3,
    ADDRESSING_ZERO_PAGE = 4,
    ADDRESSING_ZERO_PAGE_X = 5,
    ADDRESSING_ZERO_PAGE_Y = 6,
    ADDRESSING_RELATIVE = 7,
    ADDRESSING_ABSOLUTE = 8,
    ADDRESSING_ABSOLUTE_X = 9,
    ADDRESSING_ABSOLUTE_Y = 10,
    ADDRESSING_INDIRECT = 11,
    ADDRESSING_INDIRECT_X = 12,
    ADDRESSING_INDIRECT_Y = 13,

    ADDRESSING_INVALID = 0
};

static const uint8_t ADRESSING_EXTRA_BYTES[] = {
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

static enum AddressingMode getAddressingMode(uint8_t instruction)
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