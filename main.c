#include "cartridge.h"

int main(int argC, char* argV[])
{
    if(argC != 2)
        return 0;

    Cartridge* cartridge = cartridgeInsert(argV[1]);
    cartridgeRemove(cartridge);

    return 0;
}