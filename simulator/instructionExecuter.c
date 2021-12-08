#include "simulator.h"

#define rdIndex 1
#define rsIndex 2
#define rtIndex 3
#define rmIndex 4
#define imm1Index 5
#define imm2Index 6

void execute(int* registers, int* instruction, int* pc)
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
                lw(); // TODO
                break;
            case 17:
                sw(); // TODO
                break;
            case 18:
                reti(); // TODO
                break;
            case 19:
                in(); // TODO
                break;
            case 20:
                out(); // TODO
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





