#ifndef ANESTOR_SCREEN_H
#define ANESTOR_SCREEN_H

#include <stdint.h>
#include <stdbool.h>

#define NATIVE_WIDTH 256
#define NATIVE_HEIGHT 240
#define RESOLUTION_MULTIPLIER 2

typedef uint8_t NesPixel;

typedef struct screen__ Screen;

Screen* screenInit();
void screenShutdown(Screen* screen);

void screenSetPixel(Screen* screen, int x, int y, NesPixel pixel);
void screenSetScroll(Screen* screen, uint8_t xScroll, uint8_t yScroll);
void screenSetPpumask(Screen* screen, uint8_t ppumask);

void screenRefresh(Screen* screen);

bool screenIsClosed(Screen* screen);

#endif //ANESTOR_SCREEN_H
