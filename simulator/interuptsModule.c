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

void initInterupts(irq2in);
int readInterupts(int address);
