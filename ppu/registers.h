#ifndef ANESTOR_REGISTERS_H
#define ANESTOR_REGISTERS_H

typedef struct ppu_registers__ PpuRegisters;

PpuRegisters* ppuRegistersInit();
void ppuRegistersShutdown(PpuRegisters* registers);

#endif //ANESTOR_REGISTERS_H
