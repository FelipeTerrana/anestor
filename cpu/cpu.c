#include "cpu.h"

#include <stdlib.h>
#include <stdint.h>
#include "instructions.h"

struct cpu__ {
    CpuRegisters* registers;
    CpuMemory* memory;
};



static enum AddressingMode getAddressingMode__(uint8_t opcode)
{
    static const enum AddressingMode EXCEPTIONS[0x100] = {
            [0x96] = ADDRESSING_ZERO_PAGE_Y,
            [0xB6] = ADDRESSING_ZERO_PAGE_Y,
            [0xBE] = ADDRESSING_ABSOLUTE_Y,

            [0x4C] = ADDRESSING_ABSOLUTE,
            [0x6C] = ADDRESSING_INDIRECT,

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

    static const enum AddressingMode CC_BBB[0x4][0x8] = {
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
    uint8_t addressing = EXCEPTIONS[opcode];

    if(addressing != ADDRESSING_INVALID)
        return addressing;

    cc = opcode & 0x3u;
    opcode >>= 2u;

    bbb = opcode & 0x7u;

    return CC_BBB[cc][bbb];
}



static uint8_t (*getInstructionFunction__(uint8_t opcode)) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*)
{
    static uint8_t (*EXCEPTIONS[0x100]) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*) = {
            [0x96] = stx,
            [0xB6] = ldx,
            [0xBE] = ldx,

            [0x4C] = jmp,
            [0x6C] = jmp,

            [0x10] = bpl,
            [0x30] = bmi,
            [0x50] = bvc,
            [0x70] = bvs,
            [0x90] = bcc,
            [0xB0] = bcs,
            [0xD0] = bne,
            [0xF0] = beq,

            [0x00] = brk,
            [0x20] = jsr,
            [0x40] = rti,
            [0x60] = rts,

            [0x08] = php,
            [0x28] = plp,
            [0x48] = pha,
            [0x68] = pla,
            [0x88] = dey,
            [0xA8] = tay,
            [0xC8] = iny,
            [0xE8] = inx,

            [0x18] = clc,
            [0x38] = sec,
            [0x58] = cli,
            [0x78] = sei,
            [0x98] = tya,
            [0xB8] = clv,
            [0xD8] = cld,
            [0xF8] = sed,

            [0x8A] = txa,
            [0x9A] = txs,
            [0xAA] = tax,
            [0xBA] = tsx,
            [0xCA] = dex,
            [0xEA] = nop,

            [0xFF] = NULL
    };

    static uint8_t (*CC_AAA[0x4][0x8]) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*) = {
            [0x1] = {
                    [0x0] = ora,
                    [0x1] = and,
                    [0x2] = eor,
                    [0x3] = adc,
                    [0x4] = sta,
                    [0x5] = lda,
                    [0x6] = cmp,
                    [0x7] = sbc
            },
            [0x2] = {
                    [0x0] = asl,
                    [0x1] = rol,
                    [0x2] = lsr,
                    [0x3] = ror,
                    [0x4] = stx,
                    [0x5] = ldx,
                    [0x6] = dec,
                    [0x7] = inc
            },
            [0x0] = {
                    [0x1] = bit,
                    [0x4] = sty,
                    [0x5] = ldy,
                    [0x6] = cpy,
                    [0x7] = cpx,

                    [0x0] = NULL,
                    [0x2] = NULL,
                    [0x3] = NULL
            },

            [0x3] = { NULL }
    };

    uint8_t aaa, cc;
    uint8_t (*instructionFunction) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*) = EXCEPTIONS[opcode];

    if(instructionFunction != NULL)
        return instructionFunction;

    cc = opcode & 0x3u;
    opcode >>= 5u;

    aaa = opcode & 0x7u;

    return CC_AAA[cc][aaa];
}



Cpu* cpuInit(/* TODO ponteiro pra PPU / APU */Cartridge* cartridge)
{
    Cpu* cpu = malloc( sizeof(struct cpu__) );

    cpu->registers = cpuRegistersInit();
    cpu->memory = cpuMemoryInit(/* TODO ponteiro pra PPU / APU */cartridge);

    return cpu;
}



void cpuShutdown(Cpu* cpu)
{
    cpuRegistersShutdown(cpu->registers);
    cpuMemoryShutdown(cpu->memory);

    free(cpu);
}



//#include <stdio.h>
//void testDecoder(uint8_t opcode)
//{
//    const char* ADDRESSING_MODE_NAME[] = {
//        [ADDRESSING_IMPLIED] = "ADDRESSING_IMPLIED",
//        [ADDRESSING_ACCUMULATOR] = "ADDRESSING_ACCUMULATOR",
//        [ADDRESSING_IMMEDIATE] = "ADDRESSING_IMMEDIATE",
//        [ADDRESSING_ZERO_PAGE] = "ADDRESSING_ZERO_PAGE",
//        [ADDRESSING_ZERO_PAGE_X] = "ADDRESSING_ZERO_PAGE_X",
//        [ADDRESSING_ZERO_PAGE_Y] = "ADDRESSING_ZERO_PAGE_Y",
//        [ADDRESSING_RELATIVE] = "ADDRESSING_RELATIVE",
//        [ADDRESSING_ABSOLUTE] = "ADDRESSING_ABSOLUTE",
//        [ADDRESSING_ABSOLUTE_X] = "ADDRESSING_ABSOLUTE_X",
//        [ADDRESSING_ABSOLUTE_Y] = "ADDRESSING_ABSOLUTE_Y",
//        [ADDRESSING_INDIRECT] = "ADDRESSING_INDIRECT",
//        [ADDRESSING_INDIRECT_X] = "ADDRESSING_INDIRECT_X",
//        [ADDRESSING_INDIRECT_Y] = "ADDRESSING_INDIRECT_Y",
//
//        [ADDRESSING_INVALID] = "ADDRESSING_INVALID"
//    };
//
//    enum AddressingMode addressingMode = getAddressingMode__(opcode);
//
//    uint8_t (*instructionFunction) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*) =
//            getInstructionFunction__(opcode);
//
//    printf("Opcode 0x%02X\n", opcode);
//    printf("%s\n", ADDRESSING_MODE_NAME[addressingMode]);
//
//    if(instructionFunction != NULL)
//        instructionFunction(NULL, NULL, addressingMode, NULL);
//    else
//        printf("No instruction\n");
//}