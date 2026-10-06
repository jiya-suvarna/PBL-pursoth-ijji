#include <stdio.h>
#include <stdlib.h>

#include "sayam_core/cpu.h"
#include "sayam_core/memory.h"
int main(void)

{
    int choice;

    initCPU();
    initMemory();

    while (1)
    {
        printf("\n====================================\n");
        printf("       TEAM LEADER SIMULATOR\n");
        printf("====================================\n");
        printf("1. Test CPU + Memory\n");
        printf("2. View CPU Status\n");
        printf("3. View Memory\n");
        printf("0. Exit\n");
        printf("------------------------------------\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                writeMemory(0, 10);
                writeMemory(1, 20);

                executeADD(0, 1, 2);

                printf("\nCPU + Memory operation completed.\n");
                printf("Memory[2] = %d\n", readMemory(2));
                break;

            case 2:
                displayCPU();
                break;

            case 3:
                displayMemory();
                break;

            case 0:
                printf("Exiting Team Leader Simulator.\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
