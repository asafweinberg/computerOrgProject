#include "modules.h"
#include <stdint.h>

#define memorySize 4096
#define MAX_LINE_LEN  500
#define sectorsNum 128
#define sectorSize 128

static int32_t* disk[sectorsNum];
static int diskTotalSize = sectorsNum*sectorSize;
static char* diskOutAddress;

static int interrupt1;
static int diskcmd; // 0 - no command, 1 - read sector, 2 - write to sector
static int disksector; //sector number, starting from 0
static int diskbuffer; // Memory address of a buffer containing the sector being read or written
static int diskstatus; // 0 - free to receive new command, 1 - busy handling read/write command

static unsigned long lastClockStamp; //is updated every time diskstatus goes to 1
static unsigned long currentClock;


void initDisk(char* diskin, char* diskout)
{
    char line[MAX_LINE_LEN + 1];
    int i;
    int counter = 0;
    //init hardware registers
    diskcmd = 0;
    disksector = 0;
    diskbuffer = 0;
    diskstatus = 0;
    for (i = 0; i < sectorsNum; i++) {
        // initiating each sector to be 512 bytes
        disk[i] = (int32_t*) calloc(sectorSize, sizeof(int32_t));
        if (disk[i] == NULL) {
            printf("allocaion of disc sector num: %i failed\n", i);
            exit(1);
        }
    }
    FILE* diskFile = fopen(diskin,"r");
    if (!diskFile) {
        printf("error in initDisk in reading diskin: %s\n", diskin);
        exit(1);
    }
    while(fgets(line, MAX_LINE_LEN + 1, diskFile)) 
    {
        line[8] = '\0';
        int32_t decVal = strtoll(line, NULL, 16);
        disk[(int)(counter / sectorSize)][(int)(counter % sectorSize)] = decVal; //counter/512 should be division by wholes
        counter++;
    }
    // no need to fill the rest because of calloc
    fclose(diskFile);
    diskOutAddress = diskout;
}
 
void diskUpdate()
{
    currentClock = getClockCycles();
    if (currentClock == lastClockStamp + 1025) //TODO check if 1023
    {
        executeDisk(diskcmd);
        diskstatus = 0; //free to get more instructions
        printf("%d\n", currentClock);
    }
    // diskstatus = 0; //free to get more instructions
    // diskcmd = 0;
}

int readDisk(int address)
{
    switch (address)
    {
        case IO_DISK_CMD:
            return diskcmd;
            break;
        case IO_DISK_SECTOR:
            return disksector;
            break;
        case IO_DISK_BUFFER:
            return diskbuffer;
            break;
        case IO_DISK_STATUS:
            return diskstatus;
            break;
    
        default:
            printf("readDisk got a wrong io register number: %d", address);
            break;
    }
    return -1;
}


int writeDisk(int address, int value)
{
    switch (address)
    {
        case IO_DISK_CMD:
            if (!diskstatus) {
                diskcmd = value;
                if (address != 0) {
                    diskstatus = 1;
                    lastClockStamp = getClockCycles();
                    printf("stamp: %d\n", lastClockStamp);

                }
            }
            break;
        case IO_DISK_SECTOR:
            if (!diskstatus) {
                disksector = value;
            }
            break;
        case IO_DISK_BUFFER:
            if (!diskstatus) {
                diskbuffer = value;
            }
            break;
        case IO_DISK_STATUS:
            break;
    
        default:
            printf("writeDisk got a wrong io register number: %d", address);
            return -1;
            break;
    }
    return 1;
}

void executeDisk(int actionType)
{
    switch(actionType)
    {
        case 0:
            //TODO: check what to to here
            break;
        case 1:
            readSector();
            interrupt1 = 1;
            break;
        case 2:
            writeSector();
            interrupt1 = 1;
            break;
    }
    //TODO: ask whether call to writeInterrupts could be made here instead of inside cases
}

void readSector()
{
    //TODO: check num
    int i;
    if (disksector<0 || disksector>sectorSize) {
        return;
    }
    for (i = 0; i < sectorSize; i++) {
        if (diskbuffer + i == memorySize) {
            break;
        }
        writeMemory(diskbuffer + i, disk[disksector][i]);
    }
}

void writeSector()
{
    //TODO: check num
    int i;
    if (disksector<0 || disksector>sectorSize) {
        return;
    }
    for (i = 0; i < sectorSize; i++) {
        if (diskbuffer + i == memorySize) {
            break;
        }
        disk[disksector][i] = readMemory(diskbuffer + i);
    }
}


void exitDisk() 
{
    int lastIndex;
    int i;
    lastIndex = findDiskLastIndex(); //last index that doesn't equal 0
    FILE* diskOutFile = fopen(diskOutAddress,"w");
    if (!diskOutFile) {
        printf("error in exitDisk in writing to diskout: %s\n", diskOutAddress);
        exit(1);
    }
    for(i = 0; i <= lastIndex ; i++) {
		fprintf(diskOutFile, "%08X\n", disk[i / sectorSize][i % sectorSize]);
    }
    fclose(diskOutFile);
}

int findDiskLastIndex() 
{
    int lastIndex = 0;
    int i;
    for (i = 0; i < diskTotalSize; i++) {
        if (disk[i / sectorSize][i % sectorSize] != 0) {
            lastIndex = i;
        }
    }
    return lastIndex;
}

void turnOffInterrupt1()
{
    interrupt1 = 0;
}

int hasinterrupt1()
{
    return interrupt1;
}