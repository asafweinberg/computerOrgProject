
#include "simulator.h"


int debug = false;

static int registers[regSize];
static instructionArray* instructions;

char * regoutFile;
char * traceFile;
char * hwregtraceFile;

int main(int argc, char* argv[])
{
    int pc, handlingInterupt = 0, success;
    pc = 0;

    initialization(argc, argv);
    //printInstructions();
    FILE* traceF = fopen(traceFile,"w");
    FILE* hwtraceF = fopen(hwregtraceFile,"w");

    while (true)
    {
        diskUpdate();
        if (checkinterruption(pc,handlingInterupt))
        {
            pc = getIoRegister(IO_IRQ_HANDLER);
            handlingInterupt = 1;
        }

        setImmRegisters(registers, instructions->instructionArr[pc]);
        writeTraceOutput(pc, traceF);
        if (pc < instructions->length)
        {
            if (!executeInstruction(registers, instructions->instructionArr[pc], &pc, hwtraceF, &handlingInterupt))
            {
                exitSimulator();
                fclose(traceF);
                fclose(hwtraceF);
                break;
            }
        }

        
        updateClock();
    }
}


void initSimulator(char* imemin, char* regout, char* trace, char* hwregtrace)
{
    instructions = (instructionArray*)calloc(1, sizeof(instructionArray));
    initInstructions(imemin);

    regoutFile = regout;
    traceFile = trace;
    hwregtraceFile = hwregtrace;


}

void initInstructions(char* fileName)
{
    FILE* fp;
    char* line;
    int lineLength = 14, i;
    int** tempInstructions;
    char** tempOrigInstructions;
    //char tempOrigInstructions[4096][14];
    fp = fopen(fileName, "r");
    //int origInstCount=0;

    

    if (!fp)
    {
        printf("error while open imemin file");
        exit(1);
    }

    tempInstructions = (int**)calloc(4096, sizeof(int*));
    if (!tempInstructions)
    {
        printf("error allocating memory in simulator init and cant recover");
        exit(1);
    }

    tempOrigInstructions = (char**)calloc(4096, sizeof(char*));

    if (!tempOrigInstructions)
    {
        printf("error allocating memory in simulator init and cant recover");
        exit(1);
    }

    for (i = 0; i < 4096; i++)
    {
        tempOrigInstructions[i] = (char*)calloc(14, sizeof(char));
        if (!tempOrigInstructions[i])
        {
            printf("error allocating");
            exit(1);
        }
    }

    line = (char*)calloc(14, sizeof(char));
    if (!line)
    {
        printf("error allocating line");
        exit(1);
    }

    while (fgets(line, lineLength, fp))
    {
        strcpy(tempOrigInstructions[instructions->length] , line);
        tempOrigInstructions[instructions->length][12] = '\0';

        addInstruction(tempInstructions, line, instructions->length);
        (instructions->length)++;
    }

    instructions-> instructionArr = (int**)calloc(instructions->length, sizeof(int*));
    if (!instructions->instructionArr)
    {
        printf("error allocating memory instructions in simulator init and cant recover");
        exit(1);
    }

    instructions-> originalInst = (char**)calloc(instructions->length, sizeof(char*));
    if (!instructions->originalInst)
    {
        printf("error allocating memory instructions in simulator init and cant recover");
        exit(1);
    }

   

    for (int i = 0; i < instructions->length; i++)
    {
        instructions->instructionArr[i] = tempInstructions[i];
        instructions->originalInst[i] = tempOrigInstructions[i];
    }

    fclose(fp);
}

void addInstruction(int** arr, char* line, int index)
{
    int binaryIns[48], bit;
    long long int ins;

    line[12] = '\0';
    ins = strtoll(line, NULL, 16);
    for (int i = 0; i < 48; i++)
    {
        bit = ins & 1;
        ins = ins >> 1;
        binaryIns[47 - i] = bit;
    }
    int opcode = getIntFrombinary(binaryIns, 0, 7); //40-47
    int rd = getIntFrombinary(binaryIns, 8, 11); //36-39
    int rs = getIntFrombinary(binaryIns, 12, 15); //32-35
    int rt = getIntFrombinary(binaryIns, 16, 19); //28-31
    int rm = getIntFrombinary(binaryIns, 20, 23); //24-27
    int imm1 = getIntFrombinary(binaryIns, 24, 35); //12-23
    int imm2 = getIntFrombinary(binaryIns, 36, 47); //0-11

    arr[index] = (int*)calloc(INSTRUCTION_SIZE, sizeof(int));
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

int getIntFrombinary(int* binaryIns, int start, int end)
{
    int val = 0;
    int pow = 1;
    for (int i = end; i >= start; i--)
    {
        val += binaryIns[i] * pow;
        pow *= 2;
    }
    return val;
}


int initialization(int argc, char* argv[])
{
    char* imemin, * dmemin, * diskin, * irq2in, * dmemout, * regout, * trace, * hwregtrace, * cycles, * leds, * display7seg, * diskout, * monitor, * monitorYuv;

    for (int i = 0; i < regSize; i++)
    {
        registers[i] = 0;
    }

    imemin = argv[1];
    dmemin = argv[2];
    diskin = argv[3];
    irq2in = argv[4];
    dmemout = argv[5];
    regout = argv[6];
    trace = argv[7];
    hwregtrace = argv[8];
    cycles = argv[9];
    leds = argv[10];
    display7seg = argv[11];
    diskout = argv[12];
    monitor = argv[13];
    monitorYuv = argv[14];

    initSimulator(imemin, regout, trace, hwregtrace); 
    initMemory(dmemin, dmemout);
    initDisk(diskin, diskout);
    initInterrupts(irq2in);
    initClock(cycles);
    initLeds(leds);
    initDisplay(display7seg);
    initMonitor(monitor, monitorYuv);
}

void exitSimulator()
{
    exitClock();
    exitDisplay();
    exitLeds();
    exitMemory();
    exitMonitor();
    exitDisk();
    regFileHandle();
}

void regFileHandle()
{
    int i;
    FILE* rgoutF = fopen(regoutFile,"w");

    for(i=3 ; i< regSize ; i++)
    {
		fprintf(rgoutF , "%08X\n" , registers[i]);
    }
    fclose(rgoutF);

}


void writeTraceOutput(int pc, FILE * traceF)
{
    int i;

    fprintf(traceF , "%03X " , pc);
    fprintf(traceF , "%s " , instructions->originalInst[pc]);

    for(i = 0 ; i < regSize - 1 ; i++) 
    {
        fprintf(traceF, "%08x ", registers[i]); // TODO might need changes with negatives
    }
    fprintf(traceF, "%08x\n", registers[i]); // TODO might need changes with negatives

}


void printInstructions()
{
    for (int i = 0; i < instructions->length; i++)
    {
        printInstruct(i);
    }
    printf("instruction count = %d", instructions->length);

}

void printInstruct(int index)
{
    int* ins = (instructions->instructionArr)[index];
    printf("%d rd=%d, rs=%d, rt=%d, rm=%d, imm1=%d, imm2=%d\n", ins[0], ins[1], ins[2], ins[3], ins[4], ins[5], ins[6]);
}


void printRegisters() 
{
    printf("v0=%d, a0=%d, a1=%d, a2=%d, t0=%d, t1=%d, t2=%d, s0=%d, s1=%d, s2=%d, sp=%d, ra=%d\n",
        registers[3], registers[4], registers[5], registers[6], registers[7], registers[8], registers[9],
        registers[10], registers[11], registers[12], registers[14], registers[15]);
}
