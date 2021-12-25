
#include "modules.h"

#define CLOCK_MAX 0xFFFFFFFF

int timercurrent,timerMax, timerEnabled, hasInterupt0, clks;


void initClock(cycles)
{
    clks = 0;
    timercurrent = 0;
    timerEnabled = 0;
    hasInterupt0 = 0;
}


int readClock(int address)
{
    switch (address)
    {
    case IO_CLKS:
        /* code */
        break;
    case IO_TIMER_CURRENT:
        /* code */
        break;
    case IO_TIMER_ENABLE:
        /* code */
        break;
    case IO_TIMER_MAX:
        /* code */
        break;
    
    default:
        break;
    }
}

int writeClock(int address, int value)
{

}

void exitClock()
{

}


int updateClock()
{
    if (clks == CLOCK_MAX)
    {
        clks = 0;
    }
    else
    {
        clks++;
    }


    if (timerEnabled)
    {
        if (timercurrent == timerMax)
        {
            timercurrent = 0;
            hasInterupt0 = 1;
        }
        else
        {
            if (timercurrent == 0)
            {
                // TODO: check if needs to stay 1 for the future if the cpu is busy.
                hasInterupt0 = 0;
            }
            timercurrent++;
        }
    }

}

