#include "modules.h"


#define MONITOR_LEN 256

unsigned short monitoraddr;
//short monitordata;
int8_t monitordata; //TODO: check if working
int monitorcmd;
unsigned char monitorBuffer[MONITOR_LEN][MONITOR_LEN];
char *ouputTxt, *outputYuv;




void initMonitor(char* monitor, char* monitorYuv)
{
    monitoraddr = 0;
    monitordata = 0;
    monitorcmd = 0;
    ouputTxt = monitor;
    outputYuv = monitorYuv;
        
}


//reading the monitor registers
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


//writing to the monitor registers
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
        monitordata = (int8_t)value; //TODO: convert to char
        break;
    default:
        printf("writeMonitor got a wrong io register number: %d", address);
        break;
    }
    return -1;
}

// executing a write to MONITOR_CMD register
void writePixel(int value)
{
    if(value == 1)
    {
        int row, col;
        row = monitoraddr / MONITOR_LEN;
        col = monitoraddr % MONITOR_LEN;
        if (row < 0 || col < 0 || row > MONITOR_LEN || col > MONITOR_LEN)
        {
            return;
        }
    
        monitorBuffer[monitoraddr / MONITOR_LEN][monitoraddr % MONITOR_LEN] = (unsigned char) monitordata;
    }
    
}

// printing output files of the monitor
void exitMonitor()
{
    FILE* monitorFile = fopen(ouputTxt,"w");
    FILE* monitorYuvFile = fopen(outputYuv,"w");
    int i, j;
    //char buffer [8];


    for(i = 0; i < MONITOR_LEN; i++)
    {
        for(j = 0; j < MONITOR_LEN; j++ )
        {
            fprintf(monitorFile, "%02X\n", monitorBuffer[i][j]);
            
            /*_itoa((int)(monitorBuffer[i][j]) , buffer, 2);
            fprintf(monitorYuvFile , "%s\n" , buffer);*/
        }
    }
    fwrite(monitorBuffer, 1, MONITOR_LEN * MONITOR_LEN, monitorYuvFile);

    fclose(monitorFile);
    fclose(monitorYuvFile);
}
