#include "modules.h"

int initialization(int argc, char* argv[]);
void initSimulator(char* imemin, char* regout, char* trace, char* hwregtrace);
void initInstructions(char* fileName);
void addInstruction(int** arr, char* line, int index);
int getIntFrombinary(int* binaryIns, int start, int end);
void exitSimulator();

void printInstructions();
void printInstruct(int index);



