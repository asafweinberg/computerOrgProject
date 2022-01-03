#include "modules.h"

#define rdIndex 1
#define rsIndex 2
#define rtIndex 3
#define rmIndex 4
#define imm1Index 5
#define imm2Index 6

char* IORegistersName[] = {"irq0enable", "irq1enable", "irq2enable", "irq0status", "irq1status", "irq2status", "irqhandler", "irqreturn", "clks", "leds", "display7seg", "timerenable", "timercurrent", "timermax", "diskcmd","disksector", "diskbuffer", "diskstatus", "reserved","reserved", "monitoraddr", "monitordata", "monitorcmd"};

static int32_t reserved1 = 0, reserved2 = 0;

void setImmRegisters(int* registers, int* instruction)
{
    if (instruction[imm1Index] & 0x00000800)
    {
        registers[1] = instruction[imm1Index] | 0xFFFFF000; //2's complement - handle negative
    }
    else
    {
        registers[1] = instruction[imm1Index];
    }

    if (instruction[imm2Index] & 0x00000800)
    {
        registers[2] = instruction[imm2Index] | 0xFFFFF000; //2's complement - handle negative
    }
    else
    {
        registers[2] = instruction[imm2Index];
    }
}


int executeInstruction(int* registers, int* instruction, int* pc, FILE * hwtraceF, int* handlingInterupt)
{
    int opCode;
    int rdVal, rsVal, rtVal, rmVal;



    rdVal = registers[instruction[rdIndex]];
    rsVal = registers[instruction[rsIndex]];
    rtVal = registers[instruction[rtIndex]];
    rmVal = registers[instruction[rmIndex]];

    opCode = instruction[0];

    // execute arithmetic operation
    if (0 <= opCode && opCode <= 8)
    {
        if (instruction[rdIndex] == 0)
            return true;

        registers[instruction[rdIndex]] = execArithmetic(opCode, rsVal, rtVal, rmVal);
        *pc = *pc + 1;
    }
    else if (9 <= opCode && opCode <= 14)
    {
        // TODO: check instructions defintitions in pdf: "[low bits 11:0]"?
        if (execBranch(opCode, rsVal, rtVal))
        {
            *pc = rmVal & 0xFFF;
        }
        else
        {
            *pc = *pc + 1;
        }
    }
    else
    {
        switch (instruction[0])
        {
            case 15:
                registers[instruction[rdIndex]] = *pc + 1;
                *pc = rmVal & 0xFFF;
                break;
            case 16:
                registers[instruction[rdIndex]] = execLw(rsVal, rtVal, rmVal);
                *pc = *pc + 1;
                break;
            case 17:
                execSw(rdVal, rsVal, rtVal, rmVal);
                *pc = *pc + 1;
                break;
            case 18:
                *pc = getIoRegister(IO_IRQ_RETURN);
                *handlingInterupt = 0;
                break;
            case 19:
                registers[instruction[rdIndex]] = getIoRegister(rsVal + rtVal);
                *pc = *pc + 1;
                writeHwtraceOutput(hwtraceF, 19, rsVal + rtVal, registers[instruction[rdIndex]]);
                break;
            case 20:
                setIoRegister(rsVal + rtVal, rmVal);
                *pc = *pc + 1;
                writeHwtraceOutput(hwtraceF, 20, rsVal + rtVal, rmVal);
                break;
            case 21:
                return false;
                break;

            default:
                break;
        }
    }
    return true;
}

void writeHwtraceOutput(FILE * hwF, int instT, int regNum, int data)
{
    int clock = readClock(IO_CLKS);
    fprintf(hwF, "%d ", clock);
    if(instT == 19) //in == READ
    {
	    fprintf(hwF, "READ ");
    }

    else
    {
	    fprintf(hwF, "WRITE ");
    }
    fprintf(hwF, "%s ", IORegistersName[regNum]);
    fprintf(hwF, "%08x\n", data);
}


int execArithmetic(int opCode, int rsVal, int rtVal, int rmVal)
{
    int value;
    switch (opCode)
    {
        case 0:
            value = rsVal + rtVal + rmVal;
            break;
        case 1:
            value = rsVal - rtVal - rmVal;
            break;
        case 2:
            value = rsVal * rtVal + rmVal;
            break;
        case 3:
            value = rsVal & rtVal & rmVal;
            break;
        case 4:
            value = rsVal | rtVal | rmVal;
            break;
        case 5:
            value = rsVal ^ rtVal ^ rmVal;
            break;
        case 6:
            value = rsVal << rtVal;
            break;
        case 7:
            value = (int)((unsigned int)rsVal >> rtVal);
            break;
        case 8:
            value = rsVal >> rtVal;
            break;

        default:
            break;
    }
}

int execBranch(int opCode, int rsVal, int rtVal)
{
    switch (opCode)
    {
        case 9:
            return rsVal == rtVal;
            break;
        case 10:
            return rsVal != rtVal;
            break;
        case 11:
            return rsVal < rtVal;
            break;
        case 12:
            return rsVal > rtVal;
            break;
        case 13:
            return rsVal <= rtVal;
            break;
        case 14:
            return rsVal >= rtVal;
            break;
        default:
            break;
    }
}

int execLw(int rsVal, int rtVal, int rmVal)
{
    int address = (rsVal + rtVal) & 0xFFF;
    int diskValue = readMemory(address);
    return diskValue + rmVal;
}

void execSw(int rdVal, int rsVal, int rtVal, int rmVal)
{
    int newVal = rdVal + rmVal;
    int address = (rsVal + rtVal) & 0xFFF;
    writeMemory(address, newVal);
}


int getIoRegister(int address)
{
    switch (address)
    {
        case IO_IRQ_ENABLE_0:
        case IO_IRQ_ENABLE_1:
        case IO_IRQ_ENABLE_2:
        case IO_IRQ_STATUS_0:
        case IO_IRQ_STATUS_1:
        case IO_IRQ_STATUS_2:
        case IO_IRQ_HANDLER:
        case IO_IRQ_RETURN:
            return  readInterrupts(address);
            break;
        case IO_CLKS:
        case IO_TIMER_ENABLE:
        case IO_TIMER_CURRENT:
        case IO_TIMER_MAX:
            return readClock(address);
            break;
        case IO_LEDS:
            return readLeds(address);
            break;
        case IO_DISPLAY_7_SEG:
            return readDisplay(address);
            break;
        case IO_DISK_CMD:
        case IO_DISK_SECTOR:
        case IO_DISK_BUFFER:
        case IO_DISK_STATUS:
            return readDisk(address);
            break;
        case IO_RESERVED1:
            return reserved1;
            break;
        case IO_RESERVED2:
            return reserved2;
            break;
        case IO_MONITOR_ADDR:
        case IO_MONITOR_DATA:
        case IO_MONITOR_CMD:
            return readMonitor(address);
            break;
        default:
            break;
    }
}


int setIoRegister(int address, int value)
{
    switch (address)
    {
        case IO_IRQ_ENABLE_0:
        case IO_IRQ_ENABLE_1:
        case IO_IRQ_ENABLE_2:
        case IO_IRQ_STATUS_0:
        case IO_IRQ_STATUS_1:
        case IO_IRQ_STATUS_2:
        case IO_IRQ_HANDLER:
        case IO_IRQ_RETURN:
            return  writeInterrupts(address, value);
            break;
        case IO_CLKS:
        case IO_TIMER_ENABLE:
        case IO_TIMER_CURRENT:
        case IO_TIMER_MAX:
            return writeClock(address, value);
            break;
        case IO_LEDS:
            return writeLeds(address, value);
            break;
        case IO_DISPLAY_7_SEG:
            return writeDisplay(address, value);
            break;
        case IO_DISK_CMD:
        case IO_DISK_SECTOR:
        case IO_DISK_BUFFER:
        case IO_DISK_STATUS:
            return writeDisk(address, value);
            break;
        case IO_RESERVED1:
            reserved1 = value;
            return reserved1;
            break;
        case IO_RESERVED2:
            reserved2 = value;
            return reserved2;
            break;
        case IO_MONITOR_ADDR:
        case IO_MONITOR_DATA:
        case IO_MONITOR_CMD:
            return writeMonitor(address, value);
            break;
        default:
            break;
    }
}
