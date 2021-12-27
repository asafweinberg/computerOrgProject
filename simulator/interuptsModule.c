#include <modules.h>

static int irq0enable; //IRQ0 is enabled iff set to 1. IRQ0 - timer
static int irq1enable; //IRQ1 is enabled iff set to 1. IRQ1 - hard disk
static int irq2enable; //IRQ2 is enabled iff set to 1
static int irq0status; //IRQ0 status is set to 1 when irq0 is triggered
static int irq1status; //IRQ1 status is set to 1 when irq1 is triggered
static int irq2status; //IRQ2 status is set to 1 when irq2 is triggered
static int irqhandler; //PC of interrupt handler
static int irqreturn; //PC of interrupt return address

//every clock cycle
//int irq = (irq0enable && irq0status) || (irq1enable && irq1status) || (irq2enable && irq2status);

void initInterupts(irq2in) {
    irq0enable = 0;
    irq1enable = 0;
    irq2enable = 0;
    irq0status = 0;
    irq1status = 0;
    irq2status = 0;
    irqhandler = 0;
    irqreturn = 0;
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
        irq0status = value;
        break;
    case IO_IRQ_STATUS_1:
        irq1status = value;
        break;
    case IO_IRQ_STATUS_2:
        irq2status = value;
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

int checkinterruption() {
    
}