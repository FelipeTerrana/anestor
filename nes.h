#ifndef ANESTOR_NES_H
#define ANESTOR_NES_H

typedef struct nes__ Nes;

Nes* nesBoot(const char romFilename[]);
void nesShutdown(Nes* nes);

void nesRun(Nes* nes);

#endif //ANESTOR_NES_H
