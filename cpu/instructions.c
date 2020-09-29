#include "instructions.h"

#include <stdlib.h>
#include <stdbool.h>
#include "../flag_ops.h"

#define CPU_RAM_STACK_START 0x0100

#define C_MASK 0x01u
#define Z_MASK 0x02u
#define I_MASK 0x04u
#define D_MASK 0x08u
#define B_MASK 0x30u
#define V_MASK 0x40u
#define N_MASK 0x80u

struct cpu_registers__ {
    uint8_t a;
    uint8_t x, y;
    uint8_t p;
    uint8_t s;
};



static const enum AddressingMode ADDRESSING_EXCEPTIONS[0x100] = {
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

static const enum AddressingMode ADDRESSING_CC_BBB[0x4][0x8] = {
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



static const uint16_t CLOCK_TIME_EXCEPTIONS[0x100] = {
        [0x96] = 4,
        [0xB6] = 4,
        [0xBE] = 4,

        [0x4C] = 3,
        [0x6C] = 5,

        [0x10] = 2,
        [0x30] = 2,
        [0x50] = 2,
        [0x70] = 2,
        [0x90] = 2,
        [0xB0] = 2,
        [0xD0] = 2,
        [0xF0] = 2,

        [0x00] = 7,
        [0x20] = 6,
        [0x40] = 6,
        [0x60] = 6,

        [0x08] = 3,
        [0x28] = 4,
        [0x48] = 3,
        [0x68] = 4,
        [0x88] = 2,
        [0xA8] = 2,
        [0xC8] = 2,
        [0xE8] = 2,

        [0x18] = 2,
        [0x38] = 2,
        [0x58] = 2,
        [0x78] = 2,
        [0x98] = 2,
        [0xB8] = 2,
        [0xD8] = 2,
        [0xF8] = 2,

        [0x8A] = 2,
        [0x9A] = 2,
        [0xAA] = 2,
        [0xBA] = 2,
        [0xCA] = 2,
        [0xEA] = 2,

        [0x91] = 6,
        [0x9D] = 5,
        [0x99] = 5,

        [0x86] = 3,
        [0x8E] = 4,

        [0xA6] = 3,
        [0xAE] = 4,

        [0xFF] = 0
};

static const uint16_t CLOCK_TIME_CC_BBB[0x4][0x8] = {
        [0x1] = {
                [0x0] = 6,
                [0x1] = 3,
                [0x2] = 2,
                [0x3] = 4,
                [0x4] = 5,
                [0x5] = 4,
                [0x6] = 4,
                [0x7] = 4
        },
        [0x2] = {
                [0x0] = 2,
                [0x1] = 5,
                [0x2] = 2,
                [0x3] = 6,
                [0x5] = 6,
                [0x7] = 7,

                [0x4] = 0,
                [0x6] = 0
        },
        [0x0] = {
                [0x0] = 2,
                [0x1] = 3,
                [0x3] = 4,
                [0x5] = 4,
                [0x7] = 4,

                [0x2] = 0,
                [0x4] = 0,
                [0x6] = 0
        },

        [0x3] = { 0 }
};



static uint16_t (*INSTRUCTION_EXCEPTIONS[0x100]) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*) = {
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

static uint16_t (*INSTRUCTION_CC_AAA[0x4][0x8]) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*) = {
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



void decodeInstruction(uint8_t opcode,
                       uint16_t (**instructionFunction) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*),
                       enum AddressingMode* addressingMode,
                       uint16_t* clockCycles)
{

    uint8_t aaa, bbb, cc;

    cc = opcode & 0x3u;
    bbb = (opcode & (0x7u << 2u)) >> 2u;
    aaa = (opcode & (0x7u << 5u)) >> 5u;

    *instructionFunction = INSTRUCTION_EXCEPTIONS[opcode];
    *addressingMode = ADDRESSING_EXCEPTIONS[opcode];
    *clockCycles = CLOCK_TIME_EXCEPTIONS[opcode];

    if( *instructionFunction == NULL )
        *instructionFunction = INSTRUCTION_CC_AAA[cc][aaa];

    if(*addressingMode == ADDRESSING_INVALID)
        *addressingMode = ADDRESSING_CC_BBB[cc][bbb];

    if(*clockCycles == 0)
        *clockCycles = CLOCK_TIME_CC_BBB[cc][bbb];
}



CpuRegisters* cpuRegistersInit()
{
    CpuRegisters* registers = malloc( sizeof(struct cpu_registers__) );

    registers->p = 0x34;
    registers->a = registers->x = registers->y = 0x00;
    registers->s = 0xFD;

    return registers;
}



void cpuRegistersShutdown(CpuRegisters* registers)
{
    free(registers);
}



static uint8_t getOperand__(CpuRegisters* registers,
                            CpuMemory* memory,
                            enum AddressingMode addressingMode,
                            uint8_t* extraBytes,
                            uint16_t* extraCycles)
{
    uint8_t operand;
    uint8_t addressZeroPage, addressLowerByte, addressUpperByte;
    uint16_t addressFull;

    uint16_t dummy;
    if(extraCycles == NULL)
        extraCycles = &dummy;

    *extraCycles = 0;

    switch (addressingMode)
    {
        case ADDRESSING_ACCUMULATOR:
            operand = registers->a;
            break;

        case ADDRESSING_IMMEDIATE:
            operand = extraBytes[0];
            break;

        case ADDRESSING_ZERO_PAGE:
            addressZeroPage = extraBytes[0];
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &operand);
            break;

        case ADDRESSING_ZERO_PAGE_X:
            addressZeroPage = extraBytes[0] + registers->x;
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &operand);
            break;

        case ADDRESSING_ZERO_PAGE_Y:
            addressZeroPage = extraBytes[0] + registers->y;
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &operand);
            break;

        case ADDRESSING_ABSOLUTE:
            addressFull = extraBytes[0] + (extraBytes[1] << 8u);
            *extraCycles += cpuMemoryRead(memory, addressFull, &operand);
            break;

        case ADDRESSING_ABSOLUTE_X:
            addressFull = extraBytes[0] + (extraBytes[1] << 8u);
            addressFull += registers->x;
            *extraCycles += extraBytes[1] != (addressFull >> 8u) ? 1 : 0;
            *extraCycles += cpuMemoryRead(memory, addressFull, &operand);
            break;

        case ADDRESSING_ABSOLUTE_Y:
            addressFull = extraBytes[0] + (extraBytes[1] << 8u);
            addressUpperByte = addressFull >> 8u;
            addressFull += registers->y;
            *extraCycles += addressUpperByte != (addressFull >> 8u) ? 1 : 0;
            *extraCycles += cpuMemoryRead(memory, addressFull, &operand);
            break;

        case ADDRESSING_INDIRECT_X:
            addressZeroPage = extraBytes[0] + registers->x;
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &addressLowerByte);
            addressZeroPage++;
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &addressUpperByte);
            addressFull = addressLowerByte | (addressUpperByte << 8u);
            *extraCycles += cpuMemoryRead(memory, addressFull, &operand);
            break;

        case ADDRESSING_INDIRECT_Y:
            addressZeroPage = extraBytes[0];
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &addressLowerByte);
            addressZeroPage++;
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &addressUpperByte);
            addressFull = (addressLowerByte | (addressUpperByte << 8u)) + registers->y;
            *extraCycles += addressUpperByte != (addressFull >> 8u) ? 1 : 0;
            *extraCycles += cpuMemoryRead(memory, addressFull, &operand);
            break;

        default:
            operand = 0;
    }

    return operand;
}



static void storeByte__(uint8_t byte,
                        CpuRegisters* registers,
                        CpuMemory* memory,
                        enum AddressingMode addressingMode,
                        uint8_t* extraBytes,
                        uint16_t* extraCycles)
{
    uint8_t addressZeroPage, addressLowerByte, addressUpperByte;
    uint16_t addressFull;

    uint16_t dummy;
    if(extraCycles == NULL)
        extraCycles = &dummy;

    *extraCycles = 0;

    switch (addressingMode)
    {
        case ADDRESSING_ACCUMULATOR:
            registers->a = byte;
            break;

        case ADDRESSING_ZERO_PAGE:
            addressZeroPage = extraBytes[0];
            *extraCycles += cpuMemoryWrite(memory, addressZeroPage, byte);
            break;

        case ADDRESSING_ZERO_PAGE_X:
            addressZeroPage = extraBytes[0] + registers->x;
            *extraCycles += cpuMemoryWrite(memory, addressZeroPage, byte);
            break;

        case ADDRESSING_ZERO_PAGE_Y:
            addressZeroPage = extraBytes[0] + registers->y;
            *extraCycles += cpuMemoryWrite(memory, addressZeroPage, byte);
            break;

        case ADDRESSING_ABSOLUTE:
            addressFull = extraBytes[0] + (extraBytes[1] << 8u);
            *extraCycles += cpuMemoryWrite(memory, addressFull, byte);
            break;

        case ADDRESSING_ABSOLUTE_X: // TODO check page crossing
            addressFull = extraBytes[0] + (extraBytes[1] << 8u);
            addressFull += registers->x;
            *extraCycles += cpuMemoryWrite(memory, addressFull, byte);
            break;

        case ADDRESSING_ABSOLUTE_Y: // TODO check page crossing
            addressFull = extraBytes[0] + (extraBytes[1] << 8u);
            addressFull += registers->y;
            *extraCycles += cpuMemoryWrite(memory, addressFull, byte);
            break;

        case ADDRESSING_INDIRECT_X:
            addressZeroPage = extraBytes[0] + registers->x;
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &addressLowerByte);
            addressZeroPage++;
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &addressUpperByte);
            addressFull = addressLowerByte | (addressUpperByte << 8u);
            *extraCycles += cpuMemoryWrite(memory, addressFull, byte);
            break;

        case ADDRESSING_INDIRECT_Y: // TODO check page crossing
            addressZeroPage = extraBytes[0];
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &addressLowerByte);
            addressZeroPage++;
            *extraCycles += cpuMemoryRead(memory, addressZeroPage, &addressUpperByte);
            addressFull = (addressLowerByte | (addressUpperByte << 8u)) + registers->y;
            *extraCycles += cpuMemoryWrite(memory, addressFull, byte);
            break;
    }
}



static uint16_t branch__(CpuRegisters* cpuRegisters,
                         CpuMemory* cpuMemory,
                         int8_t offset,
                         uint8_t mask,
                         uint8_t valueToBranch)
{
    if( getFlagValue(cpuRegisters->p, mask) == valueToBranch )
        return 1 + cpuMemoryBranch(cpuMemory, offset);

    else
        return 0;
}



static void stackPush__(CpuRegisters* cpuRegisters,
                        CpuMemory* cpuMemory,
                        uint8_t value)
{
    cpuMemoryWrite(cpuMemory, CPU_RAM_STACK_START + cpuRegisters->s, value);
    cpuRegisters->s--;
}



static uint8_t stackPop__(CpuRegisters* cpuRegisters,
                          CpuMemory* cpuMemory)
{
    uint8_t value;
    cpuRegisters->s++;
    cpuMemoryRead(cpuMemory, CPU_RAM_STACK_START + cpuRegisters->s, &value);

    return value;
}



static void increment__(CpuRegisters* cpuRegisters,
                        uint8_t* value,
                        int8_t incrementer)
{
    *value += incrementer;

    setFlagValue(&cpuRegisters->p, Z_MASK, *value == 0 ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK,  (int8_t) (*value) < 0 ? 1 : 0);
}



#include <stdio.h>
uint16_t adc(CpuRegisters* cpuRegisters,
            CpuMemory* cpuMemory,
            enum AddressingMode addressingMode,
            uint8_t* extraBytes)
{
//    printf("ADC\n");
    uint16_t extraCycles;
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, &extraCycles);

    uint16_t fullUnsignedResult = cpuRegisters->a + operand + getFlagValue(cpuRegisters->p, C_MASK);
    int16_t fullSignedResult = (int8_t) cpuRegisters->a + (int8_t) operand + getFlagValue(cpuRegisters->p, C_MASK);
    cpuRegisters->a = fullUnsignedResult;

    setFlagValue(&cpuRegisters->p, C_MASK, fullUnsignedResult != cpuRegisters->a ? 1 : 0);
    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->a == 0 ? 1 : 0);
    setFlagValue(&cpuRegisters->p, V_MASK, fullSignedResult != cpuRegisters->a ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, fullSignedResult < 0 ? 1 : 0);

    return extraCycles;
}



uint16_t and(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("AND\n");
    uint16_t extraCycles;
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, &extraCycles);

    cpuRegisters->a &= operand;

    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->a == 0 ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, (int8_t) cpuRegisters->a < 0 ? 1 : 0);

    return extraCycles;
}



uint16_t cmp(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("CMP\n");
    uint16_t extraCycles;
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, &extraCycles);

    setFlagValue(&cpuRegisters->p, C_MASK, cpuRegisters->a >= operand ? 1 : 0);
    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->a == operand ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, (int8_t) (cpuRegisters->a - operand) < 0 ? 1 : 0);

    return extraCycles;
}



uint16_t eor(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("EOR\n");
    uint16_t extraCycles;
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, &extraCycles);

    cpuRegisters->a ^= operand;

    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->a == 0 ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, (int8_t) cpuRegisters->a < 0 ? 1 : 0);

    return extraCycles;
}



uint16_t lda(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("LDA\n");
    uint16_t extraCycles;
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, &extraCycles);

    cpuRegisters->a = operand;

    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->a == 0 ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, (int8_t) cpuRegisters->a < 0 ? 1 : 0);

    return extraCycles;
}



uint16_t ora(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("ORA\n");
    uint16_t extraCycles;
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, &extraCycles);

    cpuRegisters->a |= operand;

    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->a == 0 ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, (int8_t) cpuRegisters->a < 0 ? 1 : 0);

    return extraCycles;
}



uint16_t sbc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("SBC\n");
    uint16_t extraCycles;
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, &extraCycles);

    uint16_t fullUnsignedResult = cpuRegisters->a - operand - (1 - getFlagValue(cpuRegisters->p, C_MASK));
    int16_t fullSignedResult = (int8_t) cpuRegisters->a - (int8_t) operand - (1 - getFlagValue(cpuRegisters->p, C_MASK));
    cpuRegisters->a = fullUnsignedResult;

    setFlagValue(&cpuRegisters->p, C_MASK, fullUnsignedResult != cpuRegisters->a ? 0 : 1);
    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->a == 0 ? 1 : 0);
    setFlagValue(&cpuRegisters->p, V_MASK, fullSignedResult != cpuRegisters->a ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, fullSignedResult < 0 ? 1 : 0);

    return extraCycles;
}



uint16_t sta(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("STA\n");
    storeByte__(cpuRegisters->a, cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);
    return 0;
}



uint16_t asl(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("ASL\n");
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    setFlagValue( &cpuRegisters->p, C_MASK, getFlagValue(operand, 1 << 7) );
    operand <<= 1;
    storeByte__(operand, cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    setFlagValue( &cpuRegisters->p, Z_MASK, cpuRegisters->a == 0 ? 1 : 0 );
    setFlagValue( &cpuRegisters->p, N_MASK, (int8_t) operand < 0 ? 1 : 0 );

    return 0;
}



uint16_t dec(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("DEC\n");
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    operand--;
    storeByte__(operand, cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    setFlagValue( &cpuRegisters->p, Z_MASK, operand == 0 ? 1 : 0 );
    setFlagValue( &cpuRegisters->p, N_MASK, (int8_t) operand < 0 ? 1 : 0 );

    return 0;
}



uint16_t inc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("INC\n");
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    operand++;
    storeByte__(operand, cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    setFlagValue( &cpuRegisters->p, Z_MASK, operand == 0 ? 1 : 0 );
    setFlagValue( &cpuRegisters->p, N_MASK, (int8_t) operand < 0 ? 1 : 0 );

    return 0;
}



uint16_t ldx(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("LDX\n");
    uint16_t extraCycles;
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, &extraCycles);

    cpuRegisters->x = operand;

    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->x == 0 ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, (int8_t) cpuRegisters->x < 0 ? 1 : 0);

    return extraCycles;
}



uint16_t lsr(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("LSR\n");
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    setFlagValue( &cpuRegisters->p, C_MASK, getFlagValue(operand, 1) );
    operand >>= 1;
    storeByte__(operand, cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    setFlagValue( &cpuRegisters->p, Z_MASK, operand == 0 ? 1 : 0 );
    setFlagValue( &cpuRegisters->p, N_MASK, (int8_t) operand < 0 ? 1 : 0 );

    return 0;
}




uint16_t rol(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("ROL\n");
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    uint8_t oldCFlag = getFlagValue(cpuRegisters->p, C_MASK);
    setFlagValue( &cpuRegisters->p, C_MASK, getFlagValue(operand, 1 << 7) );

    operand = (operand << 1) | oldCFlag;
    storeByte__(operand, cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    setFlagValue( &cpuRegisters->p, Z_MASK, cpuRegisters->a == 0 ? 1 : 0 );
    setFlagValue( &cpuRegisters->p, N_MASK, (int8_t) operand < 0 ? 1 : 0 );

    return 0;
}




uint16_t ror(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("ROR\n");
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    uint8_t oldCFlag = getFlagValue(cpuRegisters->p, C_MASK);
    setFlagValue( &cpuRegisters->p, C_MASK, getFlagValue(operand, 1) );

    operand = (operand >> 1) | (oldCFlag << 7);
    storeByte__(operand, cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    setFlagValue( &cpuRegisters->p, Z_MASK, cpuRegisters->a == 0 ? 1 : 0 );
    setFlagValue( &cpuRegisters->p, N_MASK, (int8_t) operand < 0 ? 1 : 0 );

    return 0;
}



uint16_t stx(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("STX\n");
    storeByte__(cpuRegisters->x, cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);
    return 0;
}



uint16_t bit(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("BIT\n");
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);
    uint8_t bit6 = getFlagValue(operand, 1u << 6u);
    uint8_t bit7 = getFlagValue(operand, 1u << 7u);

    setFlagValue(&cpuRegisters->p, V_MASK, bit6);
    setFlagValue(&cpuRegisters->p, N_MASK, bit7);

    setFlagValue(&cpuRegisters->p, Z_MASK, (operand & cpuRegisters->a) == 0 ? 1 : 0);

    return 0;
}



uint16_t cpx(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("CPX\n");
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    setFlagValue(&cpuRegisters->p, C_MASK, cpuRegisters->x >= operand ? 1 : 0);
    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->x == operand ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, (int8_t) (cpuRegisters->x - operand) < 0 ? 1 : 0);
    return 0;
}



uint16_t cpy(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("CPY\n");
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);

    setFlagValue(&cpuRegisters->p, C_MASK, cpuRegisters->y >= operand ? 1 : 0);
    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->y == operand ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, (int8_t) (cpuRegisters->y - operand) < 0 ? 1 : 0);
    return 0;
}



uint16_t jmp(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("JMP\n");
    if(addressingMode == ADDRESSING_ABSOLUTE)
        cpuMemoryJump(cpuMemory, extraBytes[0] + (extraBytes[1] << 8u));
    else if(addressingMode == ADDRESSING_INDIRECT)
    {
        uint8_t addressLowerByte, addressUpperByte;
        cpuMemoryRead(cpuMemory, extraBytes[0] + (extraBytes[1] << 8u), &addressLowerByte);
        extraBytes[0]++;
        cpuMemoryRead(cpuMemory, extraBytes[0] + (extraBytes[1] << 8u), &addressUpperByte);

        cpuMemoryJump(cpuMemory, addressLowerByte + (addressUpperByte << 8u));
    }

    return 0;
}



uint16_t ldy(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("LDY\n");
    uint16_t extraCycles;
    uint8_t operand = getOperand__(cpuRegisters, cpuMemory, addressingMode, extraBytes, &extraCycles);

    cpuRegisters->y = operand;

    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->y == 0 ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK, (int8_t) cpuRegisters->y < 0 ? 1 : 0);

    return extraCycles;
}



uint16_t sty(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("STY\n");
    storeByte__(cpuRegisters->y, cpuRegisters, cpuMemory, addressingMode, extraBytes, NULL);
    return 0;
}



uint16_t bcc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("BCC\n");
    return branch__(cpuRegisters, cpuMemory, (int8_t) extraBytes[0], C_MASK, 0);
}



uint16_t bcs(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("BCS\n");
    return branch__(cpuRegisters, cpuMemory, (int8_t) extraBytes[0], C_MASK, 1);
}



uint16_t beq(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("BEQ\n");
    return branch__(cpuRegisters, cpuMemory, (int8_t) extraBytes[0], Z_MASK, 1);
}



uint16_t bmi(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("BMI\n");
    return branch__(cpuRegisters, cpuMemory, (int8_t) extraBytes[0], N_MASK, 1);
}



uint16_t bne(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("BNE\n");
    return branch__(cpuRegisters, cpuMemory, (int8_t) extraBytes[0], Z_MASK, 0);
}



uint16_t bpl(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("BPL\n");
    return branch__(cpuRegisters, cpuMemory, (int8_t) extraBytes[0], N_MASK, 0);
}



uint16_t bvc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("BVC\n");
    return branch__(cpuRegisters, cpuMemory, (int8_t) extraBytes[0], V_MASK, 0);
}



uint16_t bvs(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("BVS\n");
    return branch__(cpuRegisters, cpuMemory, (int8_t) extraBytes[0], V_MASK, 1);
}



uint16_t pha(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("PHA\n");
    stackPush__(cpuRegisters, cpuMemory, cpuRegisters->a);
    return 0;
}



uint16_t php(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("PHP\n");
    stackPush__(cpuRegisters, cpuMemory, cpuRegisters->p);
    return 0;
}



uint16_t pla(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("PLA\n");
    cpuRegisters->a = stackPop__(cpuRegisters, cpuMemory);

    setFlagValue(&cpuRegisters->p, Z_MASK, cpuRegisters->a == 0 ? 1 : 0);
    setFlagValue(&cpuRegisters->p, N_MASK,  (int8_t) cpuRegisters->a < 0 ? 1 : 0);

    return 0;
}




uint16_t plp(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("PLP\n");
    cpuRegisters->p = stackPop__(cpuRegisters, cpuMemory);
    return 0;
}



uint16_t dex(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("DEX\n");
    increment__(cpuRegisters, &cpuRegisters->x, -1);
    return 0;
}



uint16_t dey(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("DEY\n");
    increment__(cpuRegisters, &cpuRegisters->y, -1);
    return 0;
}



uint16_t inx(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("INX\n");
    increment__(cpuRegisters, &cpuRegisters->x, 1);
    return 0;
}



uint16_t iny(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("INY\n");
    increment__(cpuRegisters, &cpuRegisters->y, 1);
    return 0;
}



uint16_t brk(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("BRK\n");
    uint16_t interruptHandler;
    stackPush__(cpuRegisters, cpuMemory, cpuMemoryGetPc(cpuMemory));
    stackPush__(cpuRegisters, cpuMemory, cpuRegisters->p);

    setFlagValue(&cpuRegisters->p, B_MASK, 1);
    cpuMemoryRead16(cpuMemory, IRQ_VECTOR_ADDRESS, &interruptHandler);
    cpuMemoryJump(cpuMemory, interruptHandler);

    return 0;
}



uint16_t jsr(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("JSR\n");
    uint16_t subroutineAddress = extraBytes[0] + (extraBytes[1] << 8u);
    stackPush__(cpuRegisters, cpuMemory, cpuMemoryGetPc(cpuMemory) - 1);
    cpuMemoryJump(cpuMemory, subroutineAddress);
    return 0;
}



uint16_t rti(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("RTI\n");
    uint16_t pc;
    cpuRegisters->p = stackPop__(cpuRegisters, cpuMemory);
    pc = stackPop__(cpuRegisters, cpuMemory);

    cpuMemoryJump(cpuMemory, pc);
    return 0;
}



uint16_t rts(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("RTS\n");
    uint16_t pcMinusOne;
    cpuRegisters->p = stackPop__(cpuRegisters, cpuMemory);
    pcMinusOne = stackPop__(cpuRegisters, cpuMemory);

    cpuMemoryJump(cpuMemory, pcMinusOne + 1);
    return 0;
}



uint16_t nop(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("NOP\n");
    return 0;
}



uint16_t clc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("CLC\n");
    setFlagValue(&cpuRegisters->p, C_MASK, 0);
    return 0;
}



uint16_t cld(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("CLD\n");
    setFlagValue(&cpuRegisters->p, D_MASK, 0);
    return 0;
}



uint16_t cli(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("CLI\n");
    setFlagValue(&cpuRegisters->p, I_MASK, 0);
    return 0;
}



uint16_t clv(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("CLV\n");
    setFlagValue(&cpuRegisters->p, V_MASK, 0);
    return 0;
}



uint16_t sec(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("SEC\n");
    setFlagValue(&cpuRegisters->p, C_MASK, 1);
    return 0;
}



uint16_t sed(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("SED\n");
    setFlagValue(&cpuRegisters->p, D_MASK, 1);
    return 0;
}



uint16_t sei(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
//    printf("SEI\n");
    setFlagValue(&cpuRegisters->p, I_MASK, 1);
    return 0;
}



uint16_t tax(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("TAX\n");
    return 0;
}



uint16_t tay(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("TAY\n");
    return 0;
}



uint16_t tsx(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("TSX\n");
    return 0;
}



uint16_t txa(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("TXA\n");
    return 0;
}



uint16_t txs(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("TXS\n");
    return 0;
}



uint16_t tya(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("TYA\n");
    return 0;
}