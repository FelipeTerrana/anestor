#include <iostream>

extern "C" {
    #include "nes.h"
}

int main(int argC, char* argV[])
{
    if(argC != 2)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "\t" << argV[0] << " <rom_file>.nes" << std::endl;
        return 1;
    }

    Nes* nes = nesBoot(argV[1]);
    nesRun(nes);
    nesShutdown(nes);

    return 0;
}