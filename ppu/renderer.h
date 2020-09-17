#ifndef ANESTOR_RENDERER_H
#define ANESTOR_RENDERER_H

#include "memory.h"

typedef struct ppu_renderer__ PpuRenderer;

PpuRenderer* ppuRendererInit(PpuMemory* memory);
void ppuRendererShutdown(PpuRenderer* renderer);

#endif //ANESTOR_RENDERER_H
