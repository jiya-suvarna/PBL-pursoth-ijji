#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_SIZE 64

void initMemory();
void writeMemory(int address, int value);
int readMemory(int address);
void displayMemory();
#endif
