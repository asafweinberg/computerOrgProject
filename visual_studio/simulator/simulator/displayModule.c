#include "modules.h"

int display;
FILE* fpDisplayOutput;

void initDisplay(char* outputFileName)
{
    fpDisplayOutput = fopen(outputFileName, "w");
    if (!fpDisplayOutput)
    {
        printf("error opening output file for leds with name: %s", outputFileName);
        exit(1);
    }
    display = 0;
}

int readDisplay(int address)
{
    if (address == IO_DISPLAY_7_SEG)
    {
        return display;
    }
    else
    {
        printf("error read display with register address: %d", address);
        exit(1);
        return -1;
    }
}

int writeDisplay(int address, int value)
{
    unsigned int clock = readClock(IO_CLKS);
    if (address == IO_DISPLAY_7_SEG)
    {
        display = value;
        fprintf(fpDisplayOutput, "%u %08x\n", clock, display);
    }
    else
    {
        printf("error write display with register address: %d", address);
        exit(1);
        return -1;
    }
}

void exitDisplay()
{
    fclose(fpDisplayOutput);
}
