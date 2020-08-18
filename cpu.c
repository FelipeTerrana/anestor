#include "cpu.h"

#include <stdint.h>
#include "cartridge.h"

#define CPU_RAM_SIZE 0x0800

#define PPU_REGISTERS_START 0x2000
#define PPU_REGISTERS_SIZE 0x0008

#define APU_IO_REGISTERS_START 0x4000

#define APU_IO_TEST_REGISTERS_START 0x4018

#define CPU_CARTRIDGE_SPACE_START 0x4020

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