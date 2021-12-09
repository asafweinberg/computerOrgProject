#include "base.h"

int executeInstruction(int* registers, int* instruction, int* pc);
int execArithmetic(int opCode, int rsVal, int rtVal, int rmVal);
int execBranch(int opCode, int rsVal, int rtVal);

