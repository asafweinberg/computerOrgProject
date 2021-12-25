#define memorySize 4096
#define MAX_LINE_LEN  500
#include <stdio.h>
#include <math.h>
#include <modules.h>


static int memory[memorySize];
static char* dmemoutAddress;

int initMemory(char* dmemin, char* dmemout);
int readMemory(int address);
void writeMemory(int address, int value);
void exitMemory();
int findMemLastIndex();


int initMemory(char* dmemin, char* dmemout) {
    char line[MAX_LINE_LEN+1]; //TODO: check exactly how many
    int counter;
    int i;
    char* ptr;
    FILE* memFile =fopen(dmemin,"r");
    while(fgets(line, MAX_LINE_LEN+1, memFile)) {
        int decVal = (int) strtol(line,ptr,16);
        memory[counter] = decVal;
        counter++;
    }
    if (counter<4095) {
        // need to fill the rest of the memory
        for (i=counter;i<4096;i++) {
            memory[i] = 0;
        }
    }
    fclose(memFile);
    dmemoutAddress = dmemout; //save dmemout in variable
}

int readMemory(int address) {
    if (address>4095||address<0) {
        fprintf(stderr, "error in readMemory with address number: %i", address);
        exit(1);
    }
    return memory[address];
}
void writeMemory(int address, int value) {
    if (address>4095||address<0) {
        fprintf(stderr, "error in writeMemory with address number: %i", address);
        exit(1);
    }
    memory[address] = value;
}


void exitMemory() {
    int lastIndex;
    int i;
    lastIndex = findMemLastIndex(); //last index that doesn't equal 0
    FILE* dmemoutFile =fopen(dmemoutAddress,"w");
    for(i=0 ; i<= lastIndex+1 ; i++) {
		fprintf(dmemoutFile , "%08X\n" , memory[i]);
    }
    fclose(dmemoutFile);
    free(memory);
}

int findMemLastIndex() {
    int lastIndex;
    int i;
    for (i=0;i<memorySize;i++) {
        if (memory[i]!=0) {
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