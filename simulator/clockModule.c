
#include "modules.h"

#define CLOCK_MAX 0xFFFFFFFF

unsigned int timercurrent,timerMax, timerEnabled, hasInterupt0, clks;
unsigned long clockCycles;
char* outputFileName;


void initClock(char* cycles)
{
    clks = 0;
    clockCycles = 0;
    hasInterupt0 = 0;
    timercurrent = 0;
    timerEnabled = 0;
    outputFileName = cycles;
}


int readClock(int address)
{
    switch (address)
    {
    case IO_CLKS:
        return clks;
        break;
    case IO_TIMER_CURRENT:
        return timercurrent;
        break;
    case IO_TIMER_ENABLE:
        return timerEnabled;
        break;
    case IO_TIMER_MAX:
        return timerMax;
        break;
    
    default:
        printf("readClock got a wrong io register number: %d", address);
        break;
    }
    return -1;
}

int writeClock(int address, int value)
{
    switch (address)
    {
    case IO_CLKS:
        clks = value;
        break;
    case IO_TIMER_CURRENT:
        timercurrent = value;
        break;
    case IO_TIMER_ENABLE:
        timerEnabled = value;
        break;
    case IO_TIMER_MAX:
        timerMax = value;
        break;
    
    default:
        printf("writeClock got a wrong io register number: %d", address);
        return -1;
        break;
    }
    return 1;
}

void exitClock()
{
    FILE *fp;

    fp = fopen(outputFileName, "w");
    if(!fp)
    {
        printf("error opening output file for clock with name: %s", outputFileName);
    }
    fprintf(fp, "%lu", clockCycles);
    fclose(fp);
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
    clockCycles++;


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
    return clks;
}

