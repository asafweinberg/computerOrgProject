#include "modules.h"

int32_t leds;
FILE* fpLedsOutput;

void initLeds(char* outputFileName)
{
    fpLedsOutput = fopen(outputFileName, "w");
    if (!fpLedsOutput)
    {
        printf("error opening output file for leds with name: %s", outputFileName);
        exit(1);
    }
    leds = 0;
}

int readLeds(int address)
{
    if (address == IO_LEDS)
    {
        return leds;
    }
    else
    {
        return -1;
    }
}

int writeLeds(int address, int value)
{
    unsigned int clock = readClock(IO_CLKS);
    if (address == IO_LEDS)
    {
        leds = value;
        fprintf(fpLedsOutput, "%u %08x\n", clock, leds);
        return leds;
    }
    else
    {
        return -1;
    }
}

void exitLeds()
{
    fclose(fpLedsOutput);
}
