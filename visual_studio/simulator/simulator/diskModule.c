#include "modules.h"

#define MAX_LINE_LEN  500
#define sectorsNum 128
#define sectorSize 512

static int* disk[sectorsNum];
static int diskTotalSize = sectorsNum * sectorSize;
static char* diskOutAddress;

static int diskcmd; // 0 - no command, 1 - read sector, 2 - write to sector
static int disksector; //sector number, starting from 0
static int diskbuffer; // 
static int diskstatus;


void initDisk(char* diskin, char* diskout) {
    char line[MAX_LINE_LEN + 1]; //TODO: check exactly how many
    int i;
    int counter = 0;
    char* ptr;
    for (i = 0; i < sectorsNum; i++) {
        // initiating each sector to be 512 bytes
        disk[i] = (int*)calloc(512, sizeof(int));
        if (!disk[i])
        {
            printf("error allocating memory initDisk and cant recover");
            exit(1);
        }
    }
    FILE* diskFile = fopen(diskin, "r");
    if (!diskFile) {
        printf("error in initDisk in reading diskin: %s\n", diskin);
        exit(1);
    }
    while (fgets(line, MAX_LINE_LEN + 1, diskFile))
    {
        int decVal = (int)strtol(line, &ptr, 16);
        disk[(int)(counter / sectorSize)][(int)(counter % sectorSize)] = decVal; //counter/512 should be division by wholes
        counter++;
    }
    // no need to fill the rest because of calloc
    fclose(diskFile);
    diskOutAddress = diskout;
}

void writeDisk(int address, int value)
{

}

int readDisk()
{
    return 1;
}

void exitDisk()
{
    int lastIndex;
    int i;
    lastIndex = findDiskLastIndex(); //last index that doesn't equal 0
    FILE* diskOutFile = fopen(diskOutAddress, "w");
    if (!diskOutFile) {
        printf("error in exitDisk in writing to diskout: %s\n", diskOutAddress);
        exit(1);
    }
    for (i = 0; i <= lastIndex + 1; i++) {
        fprintf(diskOutFile, "%08X\n", disk[i / sectorSize][i % sectorSize]);
    }
    fclose(diskOutFile);
    free(disk);
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