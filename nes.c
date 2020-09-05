#include "nes.h"

#include <stdlib.h>
#include "cartridge.h"
#include "cpu/cpu.h"
#include "ppu/ppu.h"

struct nes__ {
    Cartridge* cartridge;
    Cpu* cpu;
    Ppu* ppu;
};



Nes* nesBoot(const char romFilename[])
{
    Nes* nes = malloc( sizeof(struct nes__) );
    nes->cartridge = cartridgeInsert(romFilename);

    nes->ppu = ppuInit(nes->cartridge);
    nes->cpu = cpuInit( ppuGetMemory(nes->ppu), nes->cartridge );

    return nes;
}



void nesShutdown(Nes* nes)
{
    cpuShutdown(nes->cpu);
    ppuShutdown(nes->ppu);
    cartridgeRemove(nes->cartridge);

    free(nes);
}