#include "base.h"


void setImmRegisters(int* registers, int* instruction);
int executeInstruction(int* registers, int* instruction, int* pc, FILE* hwtraceF, int* handlingInterupt);
int execArithmetic(int opCode, int rsVal, int rtVal, int rmVal);
int execBranch(int opCode, int rsVal, int rtVal);
int execLw(int rsVal, int rtVal, int rmVal);
void execSw(int rdVal, int rsVal, int rtVal, int rmVal);
int getIoRegister(int address);
int setIoRegister(int address, int value);



void initMemory(char* dmemin, char* dmemout);
int readMemory(int address);
void writeMemory(int address, int value);
void exitMemory();


void initDisk(diskin, diskout);
void diskUpdate();
int readDisk(int address);
int writeDisk(int address, int value);
void executeDisk(int actionType);
void readSector();
void writeSector();
void exitDisk();
int findDiskLastIndex();
int hasinterrupt1();
void turnOffInterrupt1();




void initInterrupts(char* irq2in);
int readInterrupts(int address);
int writeInterrupts(int address, int value);
int checkinterruption();




void initClock(char* cycles);
int readClock(int address);
int writeClock(int address, int value);
void exitClock();
int updateClock();
int hasinterrupt0();
unsigned long getClockCycles();


void initLeds(char* outputFileName);
int readLeds(int address);
int writeLeds(int address, int value);
void exitLeds();



void initDisplay(char* outputFileName);
int readDisplay(int address);
int writeDisplay(int address, int value);
void exitDisplay();



void initMonitor(char* monitor, char* monitorYuv);
int readMonitor(int address);
int writeMonitor(int address, int value);
void exitMonitor();
void writePixel(int value);

