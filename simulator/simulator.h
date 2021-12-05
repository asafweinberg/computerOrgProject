#include <stdio.h>
#include <stdlib.h>


#define true 1
#define false 0
#define instructionSize 7

int initialization(int argc, char* argv[]);
void initSimulator(char* imemin, char* regout, char* trace, char* hwregtrace);
void initInstructions(char* fileName);
void addInstruction(int **arr, char *line, int index);
int getIntFrombinary(int *binaryIns, int start, int end);

void printInstructions();
void printInstruct(int index);



typedef struct instruction_Array
{
    int length;
    int **instructionArr;
} instructionArray;
