#include <stdio.h>
#include "nes.h"

int main(int argC, char* argV[])
{
    if(argC != 2)
    {
        printf("Usage:\n\t%s <rom_file>.nes\n", argV[0]);
        return 1;
    }

    Nes* nes = nesBoot(argV[1]);
    nesRun(nes);
    nesShutdown(nes);

    return 0;
}