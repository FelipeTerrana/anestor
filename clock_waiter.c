#include "clock_waiter.h"

#include <stdlib.h>
#include <time.h>



struct clock_waiter__ {
    float clockSpeedHz;
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



void clockWaiterStart(ClockWaiter* clockWaiter)
{
    clockWaiter->realClockOnStart = clock();
}



void clockWaiterFinish(ClockWaiter* clockWaiter, int cycles)
{
    clockWaiter->realClockTicksToWait = cycles * (clock_t) (CLOCKS_PER_SEC / clockWaiter->clockSpeedHz);

    while ((clock() - clockWaiter->realClockOnStart) < clockWaiter->realClockTicksToWait);
}