#include "base.h"


int executeInstruction(int* registers, int* instruction, int* pc);
int execArithmetic(int opCode, int rsVal, int rtVal, int rmVal);
int execBranch(int opCode, int rsVal, int rtVal);




void initMemory(char* dmemin, char* dmemout);
int readMemory(int address);
void writeMemory(int address, int value);
void exitMemory();


void initDisk(diskin, diskout);




void initinterrupts(irq2in);
int readinterrupts(int address);




void initClock(char* cycles);
int readClock(int address);
int writeClock(int address, int value);
void exitClock();
int updateClock();



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