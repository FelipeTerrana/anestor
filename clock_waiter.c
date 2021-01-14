#include "clock_waiter.h"

#include <stdlib.h>
#include <time.h>



struct clock_waiter__ {
    float clockSpeedHz;
    int cycles;
    clock_t realClockOnStart;
    clock_t realClockTicksToWait;
};



ClockWaiter* clockWaiterInit(float clockSpeedHz)
{
    ClockWaiter* clockWaiter = malloc(sizeof(struct clock_waiter__) );
    clockWaiter->clockSpeedHz = clockSpeedHz;

    return clockWaiter;
}



void clockWaiterShutdown(ClockWaiter* clockWaiter)
{
    free(clockWaiter);
}



void clockWaiterStart(ClockWaiter* clockWaiter, int cycles)
{
    clockWaiter->cycles = cycles;
    clockWaiter->realClockOnStart = clock();
}



void clockWaiterFinish(ClockWaiter* clockWaiter)
{
    clockWaiter->realClockTicksToWait = clockWaiter->cycles * (clock_t) (CLOCKS_PER_SEC / clockWaiter->clockSpeedHz);

    while ((clock() - clockWaiter->realClockOnStart) < clockWaiter->realClockTicksToWait);
}