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



#include <stdio.h>
uint8_t adc(CpuRegisters* cpuRegisters, CpuMemory* cpuMemory, enum AddressingMode addressingMode, uint8_t* extraBytes)
{
    printf("adc\n");
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