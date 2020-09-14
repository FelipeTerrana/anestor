#ifndef ANESTOR_INSTRUCTIONS_H
#define ANESTOR_INSTRUCTIONS_H

#include <stdint.h>
#include "memory.h"

typedef struct cpu_registers__ CpuRegisters;

#define NUM_ADDRESSING_MODES 14

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

// TODO Support unofficial opcodes

CpuRegisters* cpuRegistersInit();
void cpuRegistersShutdown(CpuRegisters* registers);

void decodeInstruction(uint8_t opcode,
                       uint16_t (**instructionFunction) (CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*),
                       enum AddressingMode* addressingMode,
                       uint16_t* clockCycles);

uint16_t adc(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t and(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t cmp(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t eor(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t lda(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t ora(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t sbc(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t sta(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t asl(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t dec(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t inc(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t ldx(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t lsr(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t rol(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t ror(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t stx(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t bit(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t cpx(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t cpy(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t jmp(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t ldy(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t sty(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);


uint16_t bcc(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t bcs(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t beq(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t bmi(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t bne(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t bpl(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t brk(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t bvc(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t bvs(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t clc(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t cld(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t cli(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t clv(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t dex(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t dey(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t inx(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t iny(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t jsr(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t nop(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t pha(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t php(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t pla(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t plp(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t rti(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t rts(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t sec(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t sed(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t sei(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

uint16_t tax(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t tay(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t tsx(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t txa(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t txs(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);
uint16_t tya(CpuRegisters*, CpuMemory*, enum AddressingMode, uint8_t*);

#endif //ANESTOR_INSTRUCTIONS_H
