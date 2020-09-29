#ifndef ANESTOR_SCREEN_H
#define ANESTOR_SCREEN_H

#include <stdint.h>
#include <stdbool.h>

#define NATIVE_WIDTH 256
#define NATIVE_HEIGHT 240
#define RESOLUTION_MULTIPLIER 2

typedef struct {
    uint8_t r, g, b;
} Pixel;

typedef struct screen__ Screen;

Screen* screenInit();
void screenShutdown(Screen* screen);

void screenSetPixel(Screen* screen, int x, int y, const Pixel* pixel);
void screenRefresh(Screen* screen);

bool screenIsClosed(Screen* screen);

#endif //ANESTOR_SCREEN_H
