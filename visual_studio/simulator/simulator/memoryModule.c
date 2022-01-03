#define memorySize 4096
#define MAX_LINE_LEN  500
#define MEMORY_LINE_LEN 10

#include <math.h>
#include <ctype.h>
#include "modules.h"
#include <stdint.h>

int findMemLastIndex();


static int32_t memory[memorySize];
static char* dmemoutAddress;


void initMemory(char* dmemin, char* dmemout)
{
    char line[MEMORY_LINE_LEN];
    int counter = 0;
    int i;

    FILE* memFile = fopen(dmemin, "r");
    if (!memFile)
    {
        printf("error in initMemory in reading dmemin: %s\n", dmemin);
        exit(1);
    }

    while (fgets(line, MEMORY_LINE_LEN, memFile))
    {
        line[8] = '\0';
        int32_t decVal = strtoll(line, NULL, 16);
        memory[counter] = decVal;
        counter++;
    }
    if (counter < memorySize - 1)
    {
        // need to fill the rest of the memory
        for (i = counter; i < memorySize; i++) {
            memory[i] = 0;
        }
    }
    fclose(memFile);
    dmemoutAddress = dmemout; //save dmemout in variable
}

int readMemory(int address) {
    if (address >= memorySize || address < 0) {
        //printf("error in readMemory with address number: %i", address);
        //exit(1);
        return; // TODO: think what to do, the requirements are not to exit
    }
    return memory[address];
}
void writeMemory(int address, int32_t value) {
    if (address >= memorySize || address < 0) {
        //printf("error in writeMemory with address number: %i", address);
        //exit(1);
        return; // TODO: think what to do, the requirements are not to exit
    }
    memory[address] = value;
}


void exitMemory() {
    int lastIndex;
    int i;
    lastIndex = findMemLastIndex(); //last index that doesn't equal 0
    FILE* dmemoutFile = fopen(dmemoutAddress, "w");
    if (!dmemoutFile) {
        printf("error in exitMemory in writing to dmemout: %s\n", dmemoutAddress);
        exit(1);
    }
    for (i = 0; i <= lastIndex; i++) {
        fprintf(dmemoutFile, "%08X\n", memory[i]);
    }
    fclose(dmemoutFile);
}

int findMemLastIndex() {
    int lastIndex = 0;
    int i;
    for (i = 0; i < memorySize; i++) {
        if (memory[i] != 0) {
            lastIndex = i;
        }
    }
    return lastIndex;
}

/*int hexaCharToDecVal(char* hexa) {
    int hexaLen;
    int decValue;
    int i;
    hexaLen = 8; //by definition
    for (i=0;i<hexaLen;i++) {
        if (hexa[i]>=48 && hexa[i]<=57) {//value by number
            decValue+=(hexa[i]-48)*pow(16,7-i); //add to sum num**16
        }
        if (hexa[i]>=65 && hexa[i]<=70) {//"A" to "F"
            decValue+=(hexa[i]-55)*pow(16,7-i); //add to sum num**16
        }
        if (hexa[i]>=97 && hexa[i]<=102) {//"a" to "f"
            decValue+=(hexa[i]-87)*pow(16,7-i); //add to sum num**16
        }
    }
    return decValue;
}*/