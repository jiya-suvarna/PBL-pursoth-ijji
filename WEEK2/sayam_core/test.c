#include <stdio.h>
#include "memory.h"
#include "cpu.h"

int main()
{
    printf("=== CPU + MEMORY TEST ===\n");

    initMemory();
    initCPU();

    printf("\n1. Writing values to memory...\n");
    writeMemory(0, 10);
    writeMemory(1, 20);

    printf("Memory[0] = %d\n", readMemory(0));
    printf("Memory[1] = %d\n", readMemory(1));

    printf("\n2. CPU ADD operation...\n");
    executeADD(0, 1, 2);

    printf("Result stored in Memory[2] = %d\n", readMemory(2));

    printf("\n3. CPU Status...\n");
    displayCPU();

    printf("\n4. Complete Memory...\n");
    displayMemory();

    printf("\n=== TEST COMPLETED ===\n");

    return 0;
}
