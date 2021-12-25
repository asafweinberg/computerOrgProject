#include "base.h"


int executeInstruction(int* registers, int* instruction, int* pc);
int execArithmetic(int opCode, int rsVal, int rtVal, int rmVal);
int execBranch(int opCode, int rsVal, int rtVal);




void initMemory(dmemin, dmemout);
int readMemory(int address);
int writeMemory(int address, int value);
void exitMemory();


void initDisk(diskin, diskout);




void initInterupts(irq2in);




void initClock(char* cycles);
int readClock(int address);
int writeClock(int address, int value);
void exitClock();
int updateClock();



void initLeds(leds);




void initDisplay(display7seg); //TODO: check what is display




void initMonitor(monitor, monitorYuv);
