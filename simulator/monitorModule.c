#include "modules.h"

#define MONITOR_LEN 256

unsigned short monitoraddr;
char monitordata;
int monitorcmd;
char* monitorBuffer[MONITOR_LEN];
char *ouputTxt, *outputYuv;




void initMonitor(char* monitor, char* monitorYuv)
{
    for (int i = 0; i < MONITOR_LEN; i++)
    {
        monitorBuffer[i] = (char*)calloc(MONITOR_LEN, sizeof(char));
        if(!monitorBuffer[i])
        {
            printf("error in initMonitor, unable to allocate memory");
            exit(1);
        }
    }

    monitoraddr = 0;
    monitordata = 0;
    monitorcmd = 0;
    
}


int readMonitor(int address)
{
    switch (address)
    {
    case IO_MONITOR_ADDR:
        return monitoraddr;
        break;
    case IO_MONITOR_CMD:
        return 0;
        break;
    case IO_MONITOR_DATA:
        return monitordata;
        break;
    default:
        printf("readMonitor got a wrong io register number: %d", address);
        break;
    }
    return -1;
}


int writeMonitor(int address, int value)
{
    switch (address)
    {
    case IO_MONITOR_ADDR:
        monitoraddr = value;
        break;
    case IO_MONITOR_CMD:
        writePixel(value);
        break;
    case IO_MONITOR_DATA:
        monitordata = value;
        break;
    default:
        printf("readMonitor got a wrong io register number: %d", address);
        break;
    }
    return -1;
}


void writePixel(int value)
{
    if(value == 1)
    {
    int row, col;
    row = monitoraddr/MONITOR_LEN;
    col = monitoraddr%MONITOR_LEN;
    if (row < 0 || col < 0 || row > MONITOR_LEN || col > MONITOR_LEN)
    {
        printf("error in getPixelData in readMonitor, row = %d, col = %d", row, col);
        exit(1);
    }
    
    monitorBuffer[monitoraddr/MONITOR_LEN][monitoraddr%MONITOR_LEN] = monitordata;
    }
    
}


void exitMonitor()
{

}
