#include <stdio.h>
#include <stdlib.h>


#define true 1
#define false 0
#define instructionSize 7
#define regSize 16


typedef struct instruction_Array
{
    int length;
    int **instructionArr;
} instructionArray;