#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LEN  500
#define MAX_LABEL_LEN 50
#define MAX_INSTRUCTIONS_COUNT 4096
#define MAX_DATA_MEMORY_LEN  4096



#define true 1
#define false 0

int labelCount=0;
int maxDataAdress=0;
char* opcodeTable[] = {"add", "sub", "mac","and", "or", "xor", "sll", "sra","srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt"};

char* registerTable[]={"$zero", "$imm1", "$imm2" ,"$v0", "$a0", "$a1","$a2", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra"};

typedef struct
{
    char label[MAX_LABEL_LEN]; 
	int address;
}labelAddress; 

labelAddress labelAddressTable[MAX_INSTRUCTIONS_COUNT];
char commandParts[7][100] = {"","","","","","",""}; // TODO change to less than 100 maybe

int dataMemory [MAX_DATA_MEMORY_LEN];

void removeComments(char* line);
void changeToLowerCase(char* line);
void skipDataInst(char* line);
int emptyLine(char* line);
void deleteWhiteSpaces(char * line);
int handleInstr(char * line , char ** potenLabel , int pass); //3 cases as we described
int opcodeNum(char* opcode);
int registerNum(char* reg);
int getLabelAddress(char* label);
void encodeCommand(char* encodedLine);
int isDataCommand(char* line);
void passOne(char * fileName);
void passTwo(char * progInst, char * instMem, char * dataMem);

// void printlabelAddressTable();


int main(int argc, char *argv[])
{
    char * progInst = argv[1];  //program.asm
    char * instMem = argv[2];   //imemin.txt
    char * dataMem = argv[3];   //dmemin.txt
     
    if(argc==4)  //program.asm imemin.txt dmemin.txt
	{
		passOne(progInst);
		passTwo(progInst, instMem, dataMem);
	}

    else
    {
        printf("error in arguments\n");
    }
}


void removeComments(char* line)
{
    char* pos;
	pos = strchr(line, '#');
	if (pos)
		(*pos) = 0;
}

void changeToLowerCase(char* line)
{
    int i;
    for (i=0; line[i]; i++)
        line[i] = tolower(line[i]);		
}

void skipDataInst(char* line)
{
    char* pos;
	pos = strchr(line, '.');
	if (pos)
		(*pos) = 0;
}

// int emptyLine(char* line)
// {
//     int i;
//     for (i=0; line[i]; i++)
//         if(line[i] != ' ' && line[i] != '\t' && line[i] != '\n') 
//         	return(false);	 
// 	return(true);			
// }

int emptyLine(char* line)
{
    if (line[0]==0)
    {
        return true;
    }
    return false;
}

void deleteWhiteSpaces(char * line)
{
    int i = 0, j = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
          line[j++] = line[i];
		i++;
	}
	line[j] = '\0';
	
}

// void sliceLabelCommand(char * line , char ** labelStr, char ** commStr)
// {
//     if
//     for (in i=0 ; i < line.lenght() ; i++)
//     {
//         if (line.charAt(i)==':')
//         {

//         }
//     }
// }

void breakCommand(char * command)
{
    char* sep = ",", * registers, * typeCommand;
    int i=1;
    // char* command;
    char commandDestroy[MAX_LINE_LEN+1];
    strcpy(commandDestroy,command);
    // printf("with dots: %s\n",commandWithDots);

    // strcpy(command, commandWithDots+1); //without ':'

    // printf("no dots: %s\n",command);

    registers=strchr(command, '$');
    typeCommand=strtok(commandDestroy,"$");

    strcpy(commandParts[0], typeCommand);
    char* token = strtok(registers, sep);
  
    // Keep printing tokens while one of the 
    // delimiters present in str[]. 
    while(token!=NULL)
	{
        strcpy(commandParts[i], token); 
        token = strtok(NULL, sep); 
        i++;
    }
	return;
}

int handleInstr(char * line , char ** potenLabel , int pass) //3 cases as we described
{
    *potenLabel="";
    char* afterLabel; //* potenCommand;
    // char noDots[MAX_LINE_LEN+1];
    char * noDots;
    char destroyLine[MAX_LINE_LEN+1];
    strcpy(destroyLine, line);
    // char* endLabel = ':';

	afterLabel = strchr(line, ':');
    noDots = afterLabel+1;
    // printf("%s",newcheck);

    // *potenLabel= strtok(line, ":");
    // potenCommand= strtok(NULL, ":");

    // printf("afterlabel string: %s\n",afterLabel);
    // printf("potenCommand string: %s\n",potenCommand);

    if (afterLabel!=NULL)  //there is a label - so now only label or both
    {
        *potenLabel = strtok(destroyLine, ":");

        // printf("label: %s\n", *potenLabel);

        if (!strcmp(afterLabel,":")) //only label
        {
            return false;
        }

        if (pass==2) 
        {   
            // strcpy(noDots,afterLabel+1);
            // printf("no dots check: %s", noDots);
            breakCommand(noDots);
        }

        return true;
    }

    else
    {
        if (pass==2)
        {
            breakCommand(line);
        }
        return true;    //only command
    }
}

int opcodeNum(char* opcode)
{
	for (int i=0;i<22;i++)
    {	    
		if(!strcmp(opcodeTable[i], opcode))
			return(i);
    }
	return (0);
}

int registerNum(char* reg)
{
	for (int i=0;i<16;i++)	
    {   
		if(!strcmp(registerTable[i], reg))
			return(i);
    }
}

int getLabelAddress(char* label)
{
	for (int i=0 ; i<labelCount ; i++)	    
		if(!strcmp(labelAddressTable[i].label, label))
			return(labelAddressTable[i].address);
	
}

void encodeCommand(char* encodedLine)
{
    long long opcode, rd, rs, rt, rm, imm1, imm2; //TODO check if one long is enough
    // short imm1, imm2;
    long long codeNum;

	opcode = opcodeNum(commandParts[0]);
	rd = registerNum(commandParts[1]);
	rs = registerNum(commandParts[2]);
	rt = registerNum(commandParts[3]);
	rm = registerNum(commandParts[4]);
	
	if(isalpha(commandParts[5][0])) //check if imm1 is label
    {
        imm1= getLabelAddress(commandParts[5]);
    }

	else
    {    
        imm1=(int)strtol(commandParts[5], NULL, 0);
        imm1= imm1 & 0xFFF;
    }

    if(isalpha(commandParts[6][0])) //check if imm2 is label
    {
        imm2= getLabelAddress(commandParts[6]);
    }

	else
    {    
        imm2 = (int)strtol(commandParts[6], NULL, 0);
        imm2 = imm2 & 0xFFF;
    }

    codeNum = (opcode<<40) | (rd<<36) | (rs<<32) | (rt<<28) | (rm<<24) | (imm1<<12) | (imm2); 
    // codeNum = codeNum | (imm1<<12) | (imm2);
		
	sprintf(encodedLine, "%012llX\n", codeNum); 
}

int isDataCommand(char* line)
{
	return(strchr(line, '.') != NULL);
}

void handleDataCommand(char* line)
{   
    char* sep = " \t";

    // Returns first token 
    strtok(line , sep);
    char* address = strtok(NULL, sep);
    char* data = strtok(NULL, sep);

    int currentAdd = (int)strtol(address, NULL, 0);
	dataMemory[currentAdd]=(int)strtol(data, NULL, 0);
			if(currentAdd > maxDataAdress)
				maxDataAdress = currentAdd;


}

void passOne(char * fileName)
{
    int i, PC=0, containsCommand ;
    char line[MAX_LINE_LEN+1]; //TODO: check exactly how many
    FILE* instFile =fopen(fileName,"r");
    char * potenLabel="";

    while(fgets (line, MAX_LINE_LEN+1, instFile ))
	{    
        // potenLabel="";

		removeComments(line);
        skipDataInst(line); 
		// changeToLowerCase(line); //TODO: check about it if there are difference in the label between lower and uper
        deleteWhiteSpaces(line);

        // printf("%s\n",line);

		if(emptyLine(line))
            continue;

            //3 cases - 1)only label 2)only instruction 3)both
        containsCommand = handleInstr(line, &potenLabel, 1);

        if(!strcmp(potenLabel, "")) //only command
        {
            PC++;
        }

        else
        {
            strcpy(labelAddressTable[labelCount].label,potenLabel); //TODO check with garibi about doubled label
			labelAddressTable[labelCount].address=PC;
			labelCount++;
            if(containsCommand) //label+command
            {
                PC++;
            }
        }
    }
    fclose(instFile);
    // printlabelAddressTable(); 

}
void passTwo(char * progInst, char * instMem, char * dataMem)
{
    char line[MAX_LINE_LEN+1]; //TODO: check exactly how many
    char originalLine[MAX_LINE_LEN+1];
    char * potenLabel, encodedLine[100]; //CHECK EXACTLY
    int containsCommand, i;
    FILE* progFile = fopen(progInst,"r");
    FILE* instFile = fopen(instMem,"w");
    FILE* dataFile = fopen(dataMem,"w");

    while(fgets (line, MAX_LINE_LEN+1, progFile))
	{    
        strcpy(originalLine,line);
		removeComments(line);
        // skipDataInst(line); 
		// changeToLowerCase(line);
        deleteWhiteSpaces(line);
        
		if(emptyLine(line))
            continue;
        if (isDataCommand(line))
        {
            handleDataCommand(originalLine);
            continue;
        }

        containsCommand = handleInstr(line,&potenLabel,2);

        if(containsCommand) 
        {
            encodeCommand(encodedLine);
            fprintf(instFile, "%s", encodedLine); 
        }

    }

    for(i=0 ; i<= maxDataAdress ; i++) //TODO: check from where to write in file
		fprintf(dataFile , "%08X\n" , dataMemory[i]);

    fclose(instFile);
	fclose(instFile);
	fclose(dataFile);
}

// void printlabelAddressTable()
// {
//     for(int i=0 ; i<labelCount ; i++)
//     {
//         printf("label: %s addr: %d \n", labelAddressTable[i].label, labelAddressTable[i].address);
//     }
// }