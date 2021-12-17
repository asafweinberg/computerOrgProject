#include "base.h"

#define rdIndex 1
#define rsIndex 2
#define rtIndex 3
#define rmIndex 4
#define imm1Index 5
#define imm2Index 6

int executeInstruction(int* registers, int* instruction, int* pc)
{
    int opCode;
    int rdVal, rsVal, rtVal, rmVal;

    registers[1] = instruction[imm1Index];
    registers[2] = instruction[imm2Index];

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
                *pc = rmVal && 0xFFF;
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
                *pc = getIoRegister(7); // TODO: implement on IO module or add implementation here and send directly to modules?
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
    int diskValue = memRead(rsVal+rtVal); // TODO: implement on disk module
    return diskValue + rmVal;
}

void execSw(int rdVal, int rsVal, int rtVal, int rmVal)
{
    int newVal = rdVal + rmVal; 
    memWrite(rsVal + rtVal); // TODO: implement on disk module
}
