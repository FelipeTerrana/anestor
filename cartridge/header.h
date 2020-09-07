#ifndef ANESTOR_HEADER_H
#define ANESTOR_HEADER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define CARTRIDGE_TRAINER_MAX_SIZE 512
#define CARTRIDGE_PRG_ROM_STEP 16384
#define CARTRIDGE_CHR_ROM_STEP 8192
#define CARTRIDGE_CHR_RAM_STEP 8192
#define CARTRIDGE_TITLE_SIZE 128

typedef struct cartridge_header__ CartridgeHeader;

CartridgeHeader* cartridgeHeaderInsert(FILE* romFile);
void cartridgeHeaderRemove(CartridgeHeader* header);

uint16_t cartridgeHeaderGetMapper(const CartridgeHeader* header);
size_t cartridgeHeaderGetPrgRomSize(const CartridgeHeader* header);
size_t cartridgeHeaderGetChrRomSize(const CartridgeHeader* header);
bool cartridgeHeaderContainsTrainer(const CartridgeHeader* header);

#endif //ANESTOR_HEADER_H
