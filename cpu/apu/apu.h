#ifndef ANESTOR_APU_H
#define ANESTOR_APU_H

#define APU_REGISTERS_SIZE 0x18

typedef struct apu__ Apu;

Apu* apuInit();
void apuShutdown(Apu* apu);

#endif //ANESTOR_APU_H
