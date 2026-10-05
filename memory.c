#include "memory.h"
#include <stdio.h>

int memory[MEMORY_SIZE];

void initMemory()
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i] = 0;
    }
}

void writeMemory(int address, int value)
{
    if (address >= 0 && address < MEMORY_SIZE)
    {
        memory[address] = value;
    }
}

int readMemory(int address)
{
    if (address >= 0 && address < MEMORY_SIZE)
    {
        return memory[address];
    }

    return -1;
}

void displayMemory()
{
    printf("\nMemory Contents:\n");

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (memory[i] != 0)
        {
            printf("Address %d = %d\n", i, memory[i]);
        }
    }
}