
#include "simulator.h"


int debug = true;

static int registers[regSize];
static instructionArray* instructions;

int main(int argc, char* argv[])
{
    int pc, clock;
    pc = 0;

    initialization(argc, argv);

    while (pc < instructions -> length)
    {
        writeTraceOutput(pc);
        clock = updateClock(); // TODO: check where to update the clock
        if (checkinterruption()) //TODO
        {
            pc = startinterrupt(); //TODO
            // continue; // TODO: check when to start handling the interrupt
        } 
        if (!executeInstruction(registers, instructions->instructionArr[pc], &pc))
        {
            exitSimulator();
        }
    }
    
    exitSimulator();

    printInstructions();
}


void initSimulator(char* imemin, char* regout, char* trace, char* hwregtrace)
{
    instructions = (instructionArray*) calloc(1, sizeof(instructionArray));
    initInstructions(imemin);

    //TODO: open other 3 files for output
}

void initInstructions(char* fileName)
{
    FILE *fp;
    char* line;
    int param, lineLength;
    int** tempInstructions;
    fp = fopen(fileName, "r");
    
    if (!fp)
    {
        printf("error while open imemin file");
        exit(1);
    }

    tempInstructions = (int**)calloc(4096, sizeof(int*));
    line = (char*)calloc(14,sizeof(char));

    while ((getline(&line, &lineLength, fp)) != -1)
    {
        addInstruction(tempInstructions, line, instructions->length);
        (instructions->length)++;
    }

    instructions->instructionArr = (int**)calloc(instructions->length, sizeof(int*));
    for (int i = 0; i < instructions->length; i++)
    {
        instructions->instructionArr[i] = tempInstructions[i];
    }
    
    fclose(fp);
}

void addInstruction(int **arr, char *line, int index)
{
    int binaryIns[48], bit;
    long long int ins;

    line[12] = '\0';
    ins = strtoll(line, NULL, 16);
    for (int i = 0; i < 48; i++)
    {
        bit = ins & 1;
        ins = ins >> 1;
        binaryIns[47-i] = bit;
    }
    int opcode = getIntFrombinary(binaryIns, 0, 7); //40-47
    int rd = getIntFrombinary(binaryIns, 8, 11); //36-39
    int rs = getIntFrombinary(binaryIns, 12, 15); //32-35
    int rt = getIntFrombinary(binaryIns, 16, 19); //28-31
    int rm = getIntFrombinary(binaryIns, 20, 23); //24-27
    int imm1 = getIntFrombinary(binaryIns, 24, 35); //12-23
    int imm2 = getIntFrombinary(binaryIns, 36, 47); //0-11

    arr[index] = (int*) calloc(INSTRUCTION_SIZE, sizeof(int));
    if (!arr[index])
    {
        printf("error allocating new instruction");
        exit(1);
    }

    arr[index][0] = opcode;
    arr[index][1] = rd;
    arr[index][2] = rs;
    arr[index][3] = rt;
    arr[index][4] = rm;
    arr[index][5] = imm1;
    arr[index][6] = imm2;
}

int getIntFrombinary(int *binaryIns, int start, int end)
{
    int val = 0;
    int pow = 1;
    for(int i = end; i >= start; i--)
    {
        val += binaryIns[i] * pow;
        pow *= 2;
    }
    return val;
}


int initialization(int argc, char* argv[])
{
    char *imemin, *dmemin, *diskin, *irq2in, *dmemout, *regout, *trace, *hwregtrace, *cycles, *leds, *display7seg, *diskout, *monitor, *monitorYuv;

    // if (argc != 15)
    // {
    //     printf("error: got %d arguments instead of 14", argc-1);
    //     exit(1);
    // }
    if (debug)
    {
        imemin = "imemin.txt";
    }
    else
    {
        imemin = argv[1];
    }
    // dmemin = argv[2];
    // diskin = argv[3];
    // irq2in = argv[4];
    // dmemout = argv[5];
    // regout = argv[6];
    // trace = argv[7];
    // hwregtrace = argv[8];
    // cycles = argv[9];
    // leds = argv[10];
    // display7seg = argv[11];
    // diskout = argv[12];
    // monitor = argv[13];
    // monitorYuv = argv[14];
    
    initSimulator(imemin, regout, trace, hwregtrace); //TODO: check who's handling hwregtrace file
    initMemory(dmemin, dmemout);
    initDisk(diskin, diskout);
    initinterrupts(irq2in);
    initClock(cycles);
    initLeds(leds);
    initDisplay(display7seg); //TODO: check what is display
    initMonitor(monitor, monitorYuv);

    return true; //TODO: check return value when error
}


void printInstructions()
{
    for (int i = 0; i < instructions->length; i++)
    {
        printInstruct(i);
    }
    printf("instruction count = %d", instructions -> length);
    
}

void printInstruct(int index)
{
    int* ins = (instructions->instructionArr)[index];
    printf("%d rd=%d, rs=%d, rt=%d, rm=%d, imm1=%d, imm2=%d\n", ins[0],ins[1],ins[2],ins[3],ins[4],ins[5],ins[6]);
}

