#include "cpu.h"

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "instructions.h"

struct cpu__ {
    Apu* apu;
    CpuRegisters* registers;
    CpuMemory* memory;
};



#define MAX_OPCODE_EXTRA_BYTES 2

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



Cpu* cpuInit(PpuMemory* ppuMemory, Cartridge* cartridge)
{
    Cpu* cpu = malloc( sizeof(struct cpu__) );

    cpu->apu = apuInit();
    cpu->registers = cpuRegistersInit();
    cpu->memory = cpuMemoryInit(ppuMemory, cpu->apu, cartridge);

    return cpu;
}



void cpuShutdown(Cpu* cpu)
{
    apuShutdown(cpu->apu);
    cpuRegistersShutdown(cpu->registers);
    cpuMemoryShutdown(cpu->memory);

    free(cpu);
}



int cpuLoop(void* data)
{
    void** inputArray = data;
    Cpu* cpu = inputArray[0];
    bool* stopSignal = inputArray[1];

    uint8_t extraBytes[MAX_OPCODE_EXTRA_BYTES];

    while ( !(*stopSignal) )
    {
        uint8_t i;
        clock_t realClockTicksToWait, realClockOnStart = clock();
        uint8_t opcode = cpuMemoryFetchInstruction(cpu->memory);

        uint16_t  (*instructionFunction) ();
        enum AddressingMode addressingMode;
        uint16_t instructionClockTicks;

        decodeInstruction(opcode, &instructionFunction, &addressingMode, &instructionClockTicks);

        if(addressingMode != ADDRESSING_INVALID && instructionFunction != NULL && instructionClockTicks != 0)
        {
            for(i=0; i < ADRESSING_EXTRA_BYTES[addressingMode]; i++)
                extraBytes[i] = cpuMemoryFetchInstruction(cpu->memory);

            instructionClockTicks += instructionFunction(cpu->registers, cpu->memory, addressingMode, extraBytes);
            realClockTicksToWait = (CLOCKS_PER_SEC / NES_CPU_CLOCKS_PER_SEC) * instructionClockTicks;

            while ((clock() - realClockOnStart) < realClockTicksToWait);
        }
    }

    return 0;
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
//    uint8_t (*instructionFunction) ();
//    enum AddressingMode addressingMode;
//    uint8_t instructionClockTicks;
//
//    decodeInstruction(opcode, &instructionFunction, &addressingMode, &instructionClockTicks);
//
//    printf("Opcode 0x%02X\n", opcode);
//
//    if(instructionFunction != NULL)
//        instructionFunction(NULL, NULL, addressingMode, NULL);
//    else
//        printf("No instruction\n");
//
//    printf("%s\n", ADDRESSING_MODE_NAME[addressingMode]);
//    printf("%d ticks\n", instructionClockTicks);
//}