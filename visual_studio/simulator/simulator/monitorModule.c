#include "modules.h"


#define MONITOR_LEN 256

unsigned short monitoraddr;
short monitordata;
int monitorcmd;
unsigned char monitorBuffer[MONITOR_LEN][MONITOR_LEN];
char *ouputTxt, *outputYuv;




void initMonitor(char* monitor, char* monitorYuv)
{
   /* for (int i = 0; i < MONITOR_LEN; i++)
    {
        monitorBuffer[i] = (char*)calloc(MONITOR_LEN, sizeof(char));
        if(!monitorBuffer[i])
        {
            printf("error in initMonitor, unable to allocate memory");
            exit(1);
        }
    }*/

    monitoraddr = 0;
    monitordata = 0;
    monitorcmd = 0;
    ouputTxt = monitor;
    outputYuv = monitorYuv;
        
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
        monitoraddr = value; //TODO: convert to short
        break;
    case IO_MONITOR_CMD:
        writePixel(value);
        break;
    case IO_MONITOR_DATA:
        monitordata = (short)value; //TODO: convert to char
        break;
    default:
        printf("writeMonitor got a wrong io register number: %d", address);
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
    FILE* monitorFile = fopen(ouputTxt,"w");
    FILE* monitorYuvFile = fopen(outputYuv,"w");
    int i, j;
    //char buffer [8];


    for(i=0 ; i<MONITOR_LEN ; i++)
    {
        for(j=0 ; j<MONITOR_LEN ; j++ )
        {
            fprintf(monitorFile , "%02X\n" , monitorBuffer[i][j]);
            
            /*_itoa((int)(monitorBuffer[i][j]) , buffer, 2);
            fprintf(monitorYuvFile , "%s\n" , buffer);*/
        }
    }
    fwrite(monitorBuffer, 1, MONITOR_LEN * MONITOR_LEN, monitorYuvFile);

    fclose(monitorFile);
    fclose(monitorYuvFile);


}
