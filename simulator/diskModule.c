#include <modules.h>

#define memorySize 4096
#define MAX_LINE_LEN  500
#define sectorsNum 128
#define sectorSize 128

static int* disk[sectorsNum];
static int diskTotalSize = sectorsNum*sectorSize;
static char* diskOutAddress;

static int hasInterrupt1;
static int diskcmd; // 0 - no command, 1 - read sector, 2 - write to sector
static int disksector; //sector number, starting from 0
static int diskbuffer; // Memory address of a buffer containing the sector being read or written
static int diskstatus; // 0 - free to receive new command, 1 - busy handling read/write command

static unsigned long cycles; //TODO: check what's the right value of it


void initDisk(char* diskin, char* diskout) {
    char line[MAX_LINE_LEN+1]; //TODO: check exactly how many
    int i;
    int counter;
    char* ptr;
    //init hardware registers
    diskcmd = 0;
    disksector = 0;
    diskbuffer = 0;
    diskstatus = 0;
    for (i=0;i<sectorsNum;i++) {
        // initiating each sector to be 512 bytes
        disk[i] = (int*) calloc(sectorSize, sizeof(int));
        if (disk[i]==NULL) {
            printf("allocaion of disc sector num: %i failed\n", i);
            exit(1);
        }
    }
    FILE* diskFile =fopen(diskin,"r");
    if (!diskFile) {
        printf("error in initDisk in reading diskin: %s\n", diskin);
        exit(1);
    }
    while(fgets(line, MAX_LINE_LEN+1, diskFile)) {
        int decVal = (int) strtol(line,ptr,16);
        disk[counter/sectorSize][counter%sectorSize] = decVal; //counter/512 should be division by wholes
        counter++;
    }
    // no need to fill the rest because of calloc
    fclose(diskFile);
    diskOutAddress = diskout;
}

int readClock(int address)
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
        printf("readClock got a wrong io register number: %d", address);
        break;
    }
    return -1;
}


int writeDisk(int address, int value)
{
    switch (address)
    {
    case IO_DISK_CMD:
        executeDisk(value);
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
        printf("writeClock got a wrong io register number: %d", address);
        return -1;
        break;
    }
    return 1;
}

int executeDisk(int actionType) {
    switch(actionType)
    {
        case 0:
            diskcmd = 0; //TODO: when does it happen?
            return 0;
            break;
        case 1:
            diskcmd = 1;
            return readDisk(sectorId, bufferNum, bufferNum);
            break;
        case 2:
            diskcmd = 2;
            return writeDisk(sectorId, bufferNum);
    }
}

int readSector(int sectorId, int bufferNum) {
    int i;
    int clock;
    if (!diskstatus) {//means that disk is available
        clock = readClock(IO_CLKS);
        disksector = sectorId;
        diskbuffer = bufferNum;
        diskcmd = 1;
        //now execute
        for (i=0;i<sectorSize;i++) {
            if (bufferNum+i==memorySize) {
                break;
            }
            writeMemory(bufferNum+i, disk[sectorId][i]);
        }
        
    }

}

int writeSector(int sectorId, int bufferNum) {
    int i;
    int clock;
    if (!diskstatus) {//means that disk is available
        clock = readClock(IO_CLKS);
        disksector = sectorId;
        diskbuffer = bufferNum;
        diskcmd = 1;
        //now execute
        for (i=0;i<sectorSize;i++) {
            if (bufferNum+i==memorySize) {
                break;
            }
            disk[sectorId][i] = readMemory(bufferNum+i);
        }
        
    }

}

void exitDisk() {
    int lastIndex;
    int i;
    lastIndex = findDiskLastIndex(); //last index that doesn't equal 0
    FILE* diskOutFile =fopen(diskOutAddress,"w");
    if (!diskOutFile) {
        printf("error in exitDisk in writing to diskout: %s\n", diskOutAddress);
        exit(1);
    }
    for(i=0 ; i<= lastIndex+1 ; i++) {
		fprintf(diskOutFile , "%08X\n" , disk[i/sectorSize][i%sectorSize]);
    }
    fclose(diskOutFile);
    free(disk);
}

int findDiskLastIndex() {
    int lastIndex;
    int i;
    for (i=0;i<diskTotalSize;i++) {
        if (disk[i/sectorSize][i%sectorSize]!=0) {
            lastIndex = i;
        }
    }
    return lastIndex;
}