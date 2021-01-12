#ifndef ANESTOR_APU_H
#define ANESTOR_APU_H

#include <stdint.h>

typedef struct apu__ Apu;

Apu* apuInit();
void apuShutdown(Apu* apu);

uint16_t apuRead(Apu* apu, uint16_t address, uint8_t* value);
uint16_t apuWrite(Apu* apu, uint16_t address, uint8_t value);

#endif //ANESTOR_APU_H
