#ifndef ANESTOR_RP2A03_H
#define ANESTOR_RP2A03_H

#include <stdint.h>
#include "../../ppu/memory/memory.h"

typedef struct rp2A03__ Rp2A03;

Rp2A03* rp2A03Init(PpuMemory* ppuMemory);
void rp2A03Shutdown(Rp2A03* rp2A03);

uint16_t rp2A03Read(Rp2A03* rp2A03, uint16_t address, uint8_t* value);
uint16_t rp2A03Write(Rp2A03* rp2A03, uint16_t address, uint8_t value);

#endif //ANESTOR_RP2A03_H
