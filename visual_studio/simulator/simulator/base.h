#include <stdio.h>
#include <stdlib.h>


#define true 1
#define false 0
#define INSTRUCTION_SIZE 7
#define regSize 16


typedef struct instruction_Array
{
    int length;
    int ** instructionArr;
    char ** originalInst;
} instructionArray;


#define IO_IRQ_ENABLE_0 0
#define IO_IRQ_ENABLE_1 1
#define IO_IRQ_ENABLE_2 2
#define IO_IRQ_STATUS_0 3
#define IO_IRQ_STATUS_1 4
#define IO_IRQ_STATUS_2 5
#define IO_IRQ_HANDLER 6
#define IO_IRQ_RETURN 7
#define IO_CLKS 8
#define IO_LEDS 9
#define IO_DISPLAY_7_SEG 10
#define IO_TIMER_ENABLE 11
#define IO_TIMER_CURRENT 12
#define IO_TIMER_MAX 13
#define IO_DISK_CMD 14
#define IO_DISK_SECTOR 15
#define IO_DISK_BUFFER 16
#define IO_DISK_STATUS 17
#define IO_RESERVED1 18
#define IO_RESERVED2 19
#define IO_MONITOR_ADDR 20
#define IO_MONITOR_DATA 21
#define IO_MONITOR_CMD 22
