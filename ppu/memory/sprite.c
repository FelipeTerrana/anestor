#include "sprite.h"

#include <stdlib.h>
#include "../../flag_ops.h"

#define SPRITE_SIZE 4

#define Y_POSITION_INDEX  0
#define TILE_NUMBER_INDEX 1
#define ATTRIBUTES_INDEX  2
#define X_POSITION_INDEX  3

#define PALETTE_MASK 0x03
#define IS_BEHIND_BACKGROUND_MASK 0x20
#define IS_FLIPPED_HORIZONTALLY_MASK 0x40
#define IS_FLIPPED_VERTICALLY_MASK 0x80

void spriteInit(Sprite* sprite, uint8_t index, const uint8_t oam[])
{
    sprite->yPosition = oam[SPRITE_SIZE * index + Y_POSITION_INDEX];
    sprite->tileNumber = oam[SPRITE_SIZE * index + TILE_NUMBER_INDEX];
    sprite->attributes = oam[SPRITE_SIZE * index + ATTRIBUTES_INDEX];
    sprite->xPosition = oam[SPRITE_SIZE * index + X_POSITION_INDEX];
}



uint8_t spriteGetXPosition(Sprite* sprite)
{
    return sprite->xPosition;
}



uint8_t spriteGetYPosition(Sprite* sprite)
{
    return sprite->yPosition;
}



uint8_t spriteGetTileNumber(Sprite* sprite)
{
    return sprite->tileNumber;
}



uint8_t spriteGetPaletteNumber(Sprite* sprite)
{
    return getFlagValue(sprite->attributes, PALETTE_MASK);
}



bool spriteIsBehindBackground(Sprite* sprite)
{
    return getFlagValue(sprite->attributes, IS_BEHIND_BACKGROUND_MASK) == 1;
}



bool spriteIsFlippedHorizontally(Sprite* sprite)
{
    return getFlagValue(sprite->attributes, IS_FLIPPED_HORIZONTALLY_MASK) == 1;
}



bool spriteIsFlippedVertically(Sprite* sprite)
{
    return getFlagValue(sprite->attributes, IS_FLIPPED_VERTICALLY_MASK) == 1;
}