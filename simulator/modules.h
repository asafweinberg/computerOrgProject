#include "base.h"


int executeInstruction(int* registers, int* instruction, int* pc);
int execArithmetic(int opCode, int rsVal, int rtVal, int rmVal);
int execBranch(int opCode, int rsVal, int rtVal);




void initMemory(char* dmemin, char* dmemout);
int readMemory(int address);
void writeMemory(int address, int value);
void exitMemory();


void initDisk(diskin, diskout);




void initInterupts(irq2in);
int readInterupts(int address);




void initClock(char* cycles);
int readClock(int address);
int writeClock(int address, int value);
void exitClock();
int updateClock();



void initLeds(char* outputFileName);
int readLeds(int address);
int writeLeds(int address, int value);
void exitLeds();



void initDisplay(display7seg); //TODO: check what is display




void initMonitor(monitor, monitorYuv);
