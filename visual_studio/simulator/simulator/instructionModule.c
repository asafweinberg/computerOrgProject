#include "modules.h"

#define rdIndex 1
#define rsIndex 2
#define rtIndex 3
#define rmIndex 4
#define imm1Index 5
#define imm2Index 6

//TODO: DELETE
void printInstruction(int* instruction);



int executeInstruction(int* registers, int* instruction, int* pc)
{
    int opCode;
    int rdVal, rsVal, rtVal, rmVal;

    if(instruction[imm1Index] & 0x00000800)
        registers[1] = instruction[imm1Index] | 0xFFFFF000;

    if (instruction[imm2Index] & 0x00000800)
        registers[2] = instruction[imm2Index];

    rdVal = registers[instruction[rdIndex]];
    rsVal = registers[instruction[rsIndex]];
    rtVal = registers[instruction[rtIndex]];
    rmVal = registers[instruction[rmIndex]];

    opCode = instruction[0];
    //TODO: delete
    if (opCode == 0)
    {
        int sddf = 0;
    }
    printInstruction(instruction);
    // execute arithmetic operation
    if (0 <= opCode && opCode <= 8)
    {
        if (instruction[rdIndex] == 0)
            return true;

        registers[instruction[rdIndex]] = execArithmetic(opCode, rsVal, rtVal, rmVal);
        *pc = *pc + 1;
    }
    else if (9 <= opCode && opCode <= 13)
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
            registers[instruction[rdIndex]] = execLw(rsVal, rtVal, rmVal); // TODO: implement on memory
            *pc = *pc + 1;
            break;
        case 17:
            execSw(rdVal, rsVal, rtVal, rmVal); // TODO: implement on memory
            *pc = *pc + 1;
            break;
        case 18:
            *pc = getIoRegister(IO_IRQ_RETURN); // TODO: implement on IO module or add implementation here and send directly to modules?
            break;
        case 19:
            registers[instruction[rdIndex]] = getIoRegister(rsVal + rtVal); // TODO: implement function 
            *pc = *pc + 1;
            break;
        case 20:
            setIoRegister(rsVal + rtVal, rmVal); // TODO: implement function 
            *pc = *pc + 1;
            break;
        case 21:
            return false;
            break;

        default:
            printf("error: opcode does not exists\n");
            printf("on pc = %d, opcode = %d\n", *pc, opCode);
            exit(1);
            break;
        }
    }
    return true;
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
        printf("error in execBranch");
        exit(1);
        break;
    }
}

int execLw(int rsVal, int rtVal, int rmVal)
{
    int diskValue = readMemory(rsVal + rtVal);
    return diskValue + rmVal;
}

void execSw(int rdVal, int rsVal, int rtVal, int rmVal)
{
    int newVal = rdVal + rmVal;
    writeMemory(rsVal + rtVal, newVal);
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
        //return  readinterrupts(address);
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
        return readDisplay(address); //TODO
        break;
    case IO_DISK_CMD:
    case IO_DISK_SECTOR:
    case IO_DISK_BUFFER:
    case IO_DISK_STATUS:
        //return readDisk(address); //TODO
        break;
    case IO_RESERVED1:
    case IO_RESERVED2:
        //return readReserved(address); //TODO
        break;
    case IO_MONITOR_ADDR:
    case IO_MONITOR_DATA:
    case IO_MONITOR_CMD:
        return readMonitor(address); //TODO
        break;
    default:
        printf("error on getIoRegister, requested address is %d", address);
        exit(1);
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
        //return  writeinterrupts(address, value);
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
        return writeDisplay(address, value); //TODO
        break;
    case IO_DISK_CMD:
    case IO_DISK_SECTOR:
    case IO_DISK_BUFFER:
    case IO_DISK_STATUS:
        //return writeDisk(address, value); //TODO
        break;
    case IO_RESERVED1:
    case IO_RESERVED2:
        //return writeReserved(address, value); //TODO
        break;
    case IO_MONITOR_ADDR:
    case IO_MONITOR_DATA:
    case IO_MONITOR_CMD:
        return writeMonitor(address, value); //TODO
        break;
    default:
        printf("error on setIoRegister, requested address is %d, value is: %d", address, value);
        exit(1);
        break;
    }
}

void printInstruction(int* instruction)
{
    int* ins = instruction;
    printf("%d rd=%d, rs=%d, rt=%d, rm=%d, imm1=%d, imm2=%d\n", ins[0], ins[1], ins[2], ins[3], ins[4], ins[5], ins[6]);
}