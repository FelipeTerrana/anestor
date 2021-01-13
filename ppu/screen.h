#ifndef ANESTOR_SCREEN_H
#define ANESTOR_SCREEN_H

#include <stdint.h>
#include <stdbool.h>

#define NATIVE_WIDTH 256
#define NATIVE_HEIGHT 240
#define RESOLUTION_MULTIPLIER 2

enum PixelType {
    BACKGROUND = 0,
    SPRITE = 1
};

typedef struct {
    uint8_t value;
    enum PixelType type;
    bool isTransparent;

    // type == SPRITE only
    uint8_t priority;
    bool isBehindBackground;
} NesPixel;

typedef struct screen__ Screen;

Screen* screenInit();
void screenShutdown(Screen* screen);

bool screenSetPixel(Screen* screen, int x, int y, const NesPixel* pixel);
void screenSetScroll(Screen* screen, int xScroll, int yScroll);
void screenSetPpumask(Screen* screen, uint8_t ppumask);

void screenRefresh(Screen* screen);

bool screenIsClosed(Screen* screen);

#endif //ANESTOR_SCREEN_H
