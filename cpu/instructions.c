#include "instructions.h"

#include <stdlib.h>

#define CPU_RAM_STACK_START 0x0100

struct cpu_registers__ {
    uint8_t a;
    uint8_t x, y;
    uint8_t s;
    uint8_t p;
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



static const uint8_t CLOCK_TIME_EXCEPTIONS[0x100] = {
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

static const uint8_t CLOCK_TIME_CC_BBB[0x4][0x8] = {
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



static uint8_t (*INSTRUCTION_EXCEPTIONS[0x100]) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*) = {
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

static uint8_t (*INSTRUCTION_CC_AAA[0x4][0x8]) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*) = {
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



static enum AddressingMode getAddressingMode__(uint8_t opcode)
{
    uint8_t bbb, cc;
    enum AddressingMode addressing = ADDRESSING_EXCEPTIONS[opcode];

    if(addressing != ADDRESSING_INVALID)
        return addressing;

    cc = opcode & 0x3u;
    opcode >>= 2u;

    bbb = opcode & 0x7u;

    return ADDRESSING_CC_BBB[cc][bbb];
}



static uint8_t (*getInstructionFunction__(uint8_t opcode)) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*)
{
    uint8_t aaa, cc;
    uint8_t (*instructionFunction) () = INSTRUCTION_EXCEPTIONS[opcode];

    if(instructionFunction != NULL)
        return instructionFunction;

    cc = opcode & 0x3u;
    opcode >>= 5u;

    aaa = opcode & 0x7u;

    return INSTRUCTION_CC_AAA[cc][aaa];
}



static uint8_t getClockTime__(uint8_t opcode)
{
    uint8_t bbb, cc;
    uint8_t clockTime = CLOCK_TIME_EXCEPTIONS[opcode];

    if(clockTime != 0)
        return clockTime;

    cc = opcode & 0x3u;
    opcode >>= 2u;

    bbb = opcode & 0x7u;

    return CLOCK_TIME_CC_BBB[cc][bbb];
}



void decodeInstruction(uint8_t opcode,
                       uint8_t (**instructionFunction) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*),
                       enum AddressingMode* addressingMode,
                       uint8_t* clockCycles)
{
    *instructionFunction = getInstructionFunction__(opcode);
    *addressingMode = getAddressingMode__(opcode);
    *clockCycles = getClockTime__(opcode);
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



//static uint8_t getOperand(CpuRegisters* registers, CpuMemory* memory, enum AddressingMode addressingMode, uint8_t* extraBytes)
//{
//    uint8_t operand;
//    uint8_t addressZeroPage;
//    uint16_t addressFull;
//
//    switch (addressingMode)
//    {
//        case ADDRESSING_IMMEDIATE:
//            operand = *( (uint8_t*) extraBytes );
//            break;
//
//        case ADDRESSING_ZERO_PAGE:
//            addressZeroPage = *( (uint8_t*) extraBytes );
//            operand = cpuMemoryRead(memory, addressZeroPage);
//            break;
//
//        case ADDRESSING_ZERO_PAGE_X:
//            addressZeroPage = *( (uint8_t*) extraBytes ) + registers->x;
//            operand = cpuMemoryRead(memory, addressZeroPage);
//    }
//}



#include <stdio.h>
uint8_t adc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("adc\n");
//    uint8_t rightOperand;
//
//    switch (addressingMode)
//    {
//        case ADDRESSING_IMMEDIATE:
//            rightOperand = *( (uint8_t*) extraBytes );
//            break;
//
//        case ADDRESSING_ZERO_PAGE:
//            rightOperand = cpuMemoryRead( cpuMemory, *( (uint8_t*) extraBytes ) );
//            break;
//
//        case ADDRESSING_ZERO_PAGE_X:
//            rightOperand = cpuMemoryRead( cpuMemory, *( (uint8_t*) extraBytes ) +  );
//    }

    return 0;
}



uint8_t and(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("and\n");
    return 0;
}



uint8_t asl(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("asl\n");
    return 0;
}



uint8_t bcc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("bcc\n");
    return 0;
}



uint8_t bcs(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("bcs\n");
    return 0;
}



uint8_t beq(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("beq\n");
    return 0;
}



uint8_t bit(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("bit\n");
    return 0;
}



uint8_t bmi(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("bmi\n");
    return 0;
}



uint8_t bne(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("bne\n");
    return 0;
}



uint8_t bpl(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("bpl\n");
    return 0;
}



uint8_t brk(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("brk\n");
    return 0;
}



uint8_t bvc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("bvc\n");
    return 0;
}



uint8_t bvs(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("bvs\n");
    return 0;
}



uint8_t clc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("clc\n");
    return 0;
}



uint8_t cld(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("cld\n");
    return 0;
}



uint8_t cli(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("cli\n");
    return 0;
}



uint8_t clv(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("clv\n");
    return 0;
}



uint8_t cmp(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("cmp\n");
    return 0;
}



uint8_t cpx(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("cpx\n");
    return 0;
}



uint8_t cpy(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("cpy\n");
    return 0;
}



uint8_t dec(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("dec\n");
    return 0;
}



uint8_t dex(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("dex\n");
    return 0;
}



uint8_t dey(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("dey\n");
    return 0;
}



uint8_t eor(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("eor\n");
    return 0;
}



uint8_t inc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("inc\n");
    return 0;
}



uint8_t inx(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("inx\n");
    return 0;
}



uint8_t iny(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("iny\n");
    return 0;
}



uint8_t jmp(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("jmp\n");
    return 0;
}



uint8_t jsr(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("jsr\n");
    return 0;
}



uint8_t lda(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("lda\n");
    return 0;
}



uint8_t ldx(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("ldx\n");
    return 0;
}



uint8_t ldy(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("ldy\n");
    return 0;
}



uint8_t lsr(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("lsr\n");
    return 0;
}



uint8_t nop(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("nop\n");
    return 0;
}



uint8_t ora(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("ora\n");
    return 0;
}



uint8_t pha(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("pha\n");
    return 0;
}



uint8_t php(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("php\n");
    return 0;
}



uint8_t pla(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("pla\n");
    return 0;
}




uint8_t plp(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("plp\n");
    return 0;
}




uint8_t rol(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("rol\n");
    return 0;
}




uint8_t ror(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("ror\n");
    return 0;
}



uint8_t rti(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("rti\n");
    return 0;
}



uint8_t rts(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("rts\n");
    return 0;
}



uint8_t sbc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("sbc\n");
    return 0;
}



uint8_t sec(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("sec\n");
    return 0;
}



uint8_t sed(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("sed\n");
    return 0;
}



uint8_t sei(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("sei\n");
    return 0;
}



uint8_t sta(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("sta\n");
    return 0;
}



uint8_t stx(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("stx\n");
    return 0;
}



uint8_t sty(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("sty\n");
    return 0;
}



uint8_t tax(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("tax\n");
    return 0;
}



uint8_t tay(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("tay\n");
    return 0;
}



uint8_t tsx(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("tsx\n");
    return 0;
}



uint8_t txa(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("txa\n");
    return 0;
}



uint8_t txs(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("txs\n");
    return 0;
}



uint8_t tya(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("tya\n");
    return 0;
}