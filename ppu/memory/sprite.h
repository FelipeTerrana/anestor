#ifndef ANESTOR_SPRITE_H
#define ANESTOR_SPRITE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct sprite__ Sprite;

Sprite* spriteInit(uint8_t index, uint8_t oam[]);
void spriteShutdown(Sprite* sprite);

uint8_t spriteGetXPosition(Sprite* sprite);
uint8_t spriteGetYPosition(Sprite* sprite);
uint8_t spriteGetTileNumber(Sprite* sprite);

bool spriteIsBehindBackground(Sprite* sprite);
bool spriteIsFlippedHorizontally(Sprite* sprite);
bool spriteIsFlippedVertically(Sprite* sprite);

#endif //ANESTOR_SPRITE_H
