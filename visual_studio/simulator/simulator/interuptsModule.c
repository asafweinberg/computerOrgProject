#include "modules.h"

#define MAX_LINE_LEN  500

static int irq0enable; //IRQ0 is enabled iff set to 1. IRQ0 - timer
static int irq1enable; //IRQ1 is enabled iff set to 1. IRQ1 - hard disk
static int irq2enable; //IRQ2 is enabled iff set to 1
static int irq0status; //IRQ0 status is set to 1 when irq0 is triggered
static int irq1status; //IRQ1 status is set to 1 when irq1 is triggered
static int irq2status; //IRQ2 status is set to 1 when irq2 is triggered
static int irqhandler; //PC of interrupt handler
static int irqreturn; //PC of interrupt return address
static int irq;
static int irq2Num;
static int currentIrq2Index;
static long* irq2Listings;

//every clock cycle

void initInterupts(char* irq2in) {
    char line[MAX_LINE_LEN+1];
    int counter = 0;
    char* ptr;
    irq0enable = 0;
    irq1enable = 0;
    irq2enable = 0;
    irq0status = 0;
    irq1status = 0;
    irq2status = 0;
    irqhandler = 0;
    irqreturn = 0;
    irq = 0;
    currentIrq2Index = 0;
    FILE* irq2File = fopen(irq2in,"r");
    if (!irq2File) {
        printf("error in initInterupts in reading irq2in: %s\n", irq2in);
        exit(1);
    }
    //initiate irq2 array
    //TODO: check how to write it more smoothly with reallocation
    irq2Listings = (long*) calloc(1,sizeof(long));
    if (!irq2Listings) {
        printf("error in allocating memory\n");
        exit(1);
    }
    while(fgets(line, MAX_LINE_LEN + 1, irq2File))
    {
        irq2Listings = (long*) realloc(irq2Listings,(counter+1)*sizeof(long));
        if (!irq2Listings) {
            printf("error in allocating memory\n");
            exit(1);
        }
        int decVal = (int) strtol(line, &ptr, 10);
        irq2Listings[counter] = decVal;
        counter++;
    }
    fclose(irq2File);
    irq2Num = counter;
}

int readInterrupts(int address) {
    switch (address)
    {
    case IO_IRQ_ENABLE_0:
        return irq0enable;
        break;
    case IO_IRQ_ENABLE_1:
        return irq1enable;
        break;
    case IO_IRQ_ENABLE_2:
        return irq2enable;
        break;
    case IO_IRQ_STATUS_0:
        return irq0status;
        break;
    case IO_IRQ_STATUS_1:
        return irq1status;
        break;
    case IO_IRQ_STATUS_2:
        return irq2status;
        break;
    case IO_IRQ_HANDLER:
        return irqhandler;
        break;
    case IO_IRQ_RETURN:
        return irqreturn;
        break;
    
    default:
        printf("readClock got a wrong io register number: %d", address);
        break;
    }
    return -1;
}

int writeInterrupts(int address, int value)
{
    switch (address)
    {
    case IO_IRQ_ENABLE_0:
        irq0enable = value;
        break;
    case IO_IRQ_ENABLE_1:
        irq1enable = value;
        break;
    case IO_IRQ_ENABLE_2:
        irq2enable = value;
        break;
    case IO_IRQ_STATUS_0:
        break;
    case IO_IRQ_STATUS_1:
        break;
    case IO_IRQ_STATUS_2:
        break;
    case IO_IRQ_HANDLER:
        irqhandler = value;
        break;
    case IO_IRQ_RETURN:
        irqreturn = value;
        break;

    
    default:
        printf("writeInterrupts got a wrong io register number: %d", address);
        return -1;
        break;
    }
    return 1;
}


int checkinterruption() {//get irq
    //irq==1 iff there is an interruption
    //check int0
    //check int1 - no need to do since irq1status is updated directly with writeInterrupts in diskModule
    //TODO: make sure it's clock cycles or IO_CLOCKS_REGISTER, and check if we should repeat - if so, change index
    if (currentIrq2Index==irq2Num) {
        // we are done reading irq2 from input file
        irq2status = 0;
    }
    else if (irq2Listings[currentIrq2Index]==getClockCycles()) {
        irq2status = 1;
        currentIrq2Index++;
    }
    else {//currentIrq2Index!=irq2Num but also irq2Listings[currentIrq2Index]!=getClockCycles()
        irq2status = 0;
    }
    irq0status = hasinterrupt0();
    irq1status = hasinterrupt1();
    irq = (irq0enable && irq0status) || (irq1enable && irq1status) || (irq2enable && irq2status);
    if (irq1status) {
        turnOffInterrupt1();
    }
    return irq;
}