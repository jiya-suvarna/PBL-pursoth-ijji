#ifndef CPU_H
#define CPU_H

void initCPU();
void executeADD(int address1, int address2, int resultAddress);

int getAccumulator();
int getProgramCounter();

void displayCPU();

#endif
