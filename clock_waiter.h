#ifndef ANESTOR_CLOCK_WAITER_H
#define ANESTOR_CLOCK_WAITER_H

#include <stdint.h>

#define MASTER_CLOCK_SPEED_HZ (236250000 / 11.0)

#define CPU_CLOCK_SPEED_HZ (MASTER_CLOCK_SPEED_HZ / 12.0)
#define PPU_CLOCK_SPEED_HZ (MASTER_CLOCK_SPEED_HZ / 4.0)

typedef struct clock_waiter__ ClockWaiter;

ClockWaiter* clockWaiterInit(float clockSpeedHz);
void clockWaiterShutdown(ClockWaiter* clockWaiter);

void clockWaiterStart(ClockWaiter* clockWaiter);
void clockWaiterFinish(ClockWaiter* clockWaiter, int cycles);

#endif //ANESTOR_CLOCK_WAITER_H
