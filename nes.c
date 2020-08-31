#include "nes.h"

#include <stdlib.h>
#include "cartridge.h"
#include "cpu/cpu.h"

struct nes__ {
    Cartridge* cartridge;
    Cpu* cpu;
};



Nes* nesBoot(const char romFilename[])
{
    Nes* nes = malloc( sizeof(struct nes__) );
    nes->cartridge = cartridgeInsert(romFilename);
    nes->cpu = cpuInit(nes->cartridge);

    return nes;
}



void nesShutdown(Nes* nes)
{
    cpuShutdown(nes->cpu);
    cartridgeRemove(nes->cartridge);

    free(nes);
}