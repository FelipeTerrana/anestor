#include "nes.h"

#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include "cartridge/cartridge.h"
#include "cpu/cpu.h"
#include "ppu/ppu.h"

struct nes__ {
    Cartridge* cartridge;
    Cpu* cpu;
    Ppu* ppu;
};



Nes* nesBoot(const char romFilename[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

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
    SDL_Quit();
}



void nesRun(Nes* nes)
{
    bool stopSignal = false;
    void* cpuLoopInput[2] = {nes->cpu, &stopSignal};
    void* ppuLoopInput[2] = {nes->ppu, &stopSignal};

    SDL_Thread* cpuThread = SDL_CreateThread(cpuLoop, "CPU", cpuLoopInput);
    ppuLoop(ppuLoopInput);

    SDL_WaitThread(cpuThread, NULL);
}