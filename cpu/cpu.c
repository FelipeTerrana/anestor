#include "cpu.h"

#include <stdlib.h>
#include <stdint.h>
#include "instructions.h"
#include "rp2A03/rp2A03.h"
#include "../clock_waiter.h"

//#define LOG_CPU_STATUS

struct cpu__ {
    Rp2A03* rp2A03;
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

    cpu->rp2A03 = rp2A03Init(ppuMemory);
    cpu->registers = cpuRegistersInit();
    cpu->memory = cpuMemoryInit(ppuMemory, cpu->rp2A03, cartridge);

    return cpu;
}



void cpuShutdown(Cpu* cpu)
{
    rp2A03Shutdown(cpu->rp2A03);
    cpuRegistersShutdown(cpu->registers);
    cpuMemoryShutdown(cpu->memory);

    free(cpu);
}



int cpuLoop(void* data)
{
    void** inputArray = data;
    Cpu* cpu = inputArray[0];
    bool* stopSignal = inputArray[1];

    ClockWaiter* clockWaiter = clockWaiterInit(CPU_CLOCK_SPEED_HZ);

    uint8_t extraBytes[MAX_OPCODE_EXTRA_BYTES];

    #ifdef LOG_CPU_STATUS
    FILE* log = fopen("anestor.log", "w");
    #endif

    while ( !(*stopSignal) )
    {
        uint8_t i;

        cpuCheckInterrupt(cpu->registers, cpu->memory);

        #ifdef LOG_CPU_STATUS
        fprintf(log, "0x%04X\n", cpuMemoryGetPc(cpu->memory));
        fprintf(log, "0x%02X\n", ((uint8_t*) cpu->registers)[0]);
        fprintf(log, "0x%02X\n", ((uint8_t*) cpu->registers)[1]);
        fprintf(log, "0x%02X\n", ((uint8_t*) cpu->registers)[2]);
        fprintf(log, "0x%02X\n", ((uint8_t*) cpu->registers)[3]);
        fprintf(log, "0x%02X\n", ((uint8_t*) cpu->registers)[4]);
        #endif

        uint8_t opcode = cpuMemoryFetchInstruction(cpu->memory);

        uint16_t  (*instructionFunction) ();
        enum AddressingMode addressingMode;
        uint16_t instructionClockTicks, extraClockTicks;

        decodeInstruction(opcode, &instructionFunction, &addressingMode, &instructionClockTicks);

        if(addressingMode != ADDRESSING_INVALID && instructionFunction != NULL && instructionClockTicks != 0)
        {
            clockWaiterStart(clockWaiter, instructionClockTicks);

            for(i=0; i < ADRESSING_EXTRA_BYTES[addressingMode]; i++)
                extraBytes[i] = cpuMemoryFetchInstruction(cpu->memory);

            extraClockTicks = instructionFunction(cpu->registers, cpu->memory, addressingMode, extraBytes);

            clockWaiterAddCycles(clockWaiter, extraClockTicks);
            clockWaiterFinish(clockWaiter);
        }
    }

    clockWaiterShutdown(clockWaiter);

    #ifdef LOG_CPU_STATUS
    fclose(log);
    #endif

    return 0;
}