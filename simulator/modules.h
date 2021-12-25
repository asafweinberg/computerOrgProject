#include "base.h"

int executeInstruction(int* registers, int* instruction, int* pc);
int execArithmetic(int opCode, int rsVal, int rtVal, int rmVal);
int execBranch(int opCode, int rsVal, int rtVal);


    // initSimulator(imemin, regout, trace, hwregtrace); //TODO: check who's handling hwregtrace file
    // initMemory(dmemin, dmemout);
    // initDisk(diskin, diskout);
    // initInterupts(irq2in);
    // initClock(cycles);
    // initLeds(leds);
    // initDisplay(display7seg); //TODO: check what is display
    // initMonitor(monitor, monitorYuv);


    initMemory(dmemin, dmemout);
    readMemory(int address);
    writeMemory(int address, int value);
    exitMemory();


    initDisk(diskin, diskout);




    initInterupts(irq2in);




    initClock(cycles);




    initLeds(leds);




    initDisplay(display7seg); //TODO: check what is display




    initMonitor(monitor, monitorYuv);
