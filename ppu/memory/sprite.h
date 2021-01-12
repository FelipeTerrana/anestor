#ifndef ANESTOR_SPRITE_H
#define ANESTOR_SPRITE_H

#include <stdint.h>
#include <stdbool.h>

#define NUMBER_OF_SPRITES 64

typedef struct sprite__ Sprite;

Sprite* spriteInit(uint8_t index, const uint8_t oam[]);
void spriteShutdown(Sprite* sprite);

uint8_t spriteGetXPosition(Sprite* sprite);
uint8_t spriteGetYPosition(Sprite* sprite);
uint8_t spriteGetTileNumber(Sprite* sprite);

uint8_t spriteGetPaletteNumber(Sprite* sprite);
bool spriteIsBehindBackground(Sprite* sprite);
bool spriteIsFlippedHorizontally(Sprite* sprite);
bool spriteIsFlippedVertically(Sprite* sprite);

#endif //ANESTOR_SPRITE_H
