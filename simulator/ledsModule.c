#include "modules.h"

int leds;
FILE *fpOutput;

void initLeds(char* outputFileName)
{
    fpOutput = fopen(outputFileName, "w");
    if(!fpOutput)
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
        printf("error read leds with register address: %d", address);
        exit(1);
        return -1;
    }
}

int writeLeds(int address, int value)
{
    unsigned int clock = readClock(IO_CLKS);
    if (address == IO_LEDS)
    {
        leds = value;
        fprintf(fpOutput, "%u %08X\n", clock, leds);
    }
    else
    {
        printf("error read leds with register address: %d", address);
        exit(1);
        return -1;
    }
}

void exitLeds()
{
    fclose(fpOutput);
}
