#include <stdio.h>
#include "cpu.h"
#include "memory.h"

int programCounter = 0;
int accumulator = 0;

void initCPU()
{
    programCounter = 0;
    accumulator = 0;
}

void executeADD(int address1, int address2, int resultAddress)
{
    int value1 = readMemory(address1);
    int value2 = readMemory(address2);

    accumulator = value1 + value2;

    writeMemory(resultAddress, accumulator);

    programCounter++;
}

int getAccumulator()
{
    return accumulator;
}

int getProgramCounter()
{
    return programCounter;
}

void displayCPU()
{
    printf("\nCPU Status:\n");
    printf("Program Counter = %d\n", programCounter);
    printf("Accumulator = %d\n", accumulator);
}
