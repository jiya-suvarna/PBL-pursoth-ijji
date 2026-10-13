
#include <stdio.h>
#include "schedule.h"

int main(void)
{
    Process processes[MAX];
    Process temp[MAX];
    int n, choice, quantum;
    int i, j;

    printf("===== CPU SCHEDULING SIMULATOR =====\n");

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    if (n < 1 || n > MAX)
    {
        printf("Invalid number of processes.\n");
        return 1;
    }

    input_processes(processes, n);

    do
    {
        display_menu();
        scanf("%d", &choice);

        if (choice == 4 || choice == 7)
        {
            printf("Enter time quantum: ");
            scanf("%d", &quantum);

            if (quantum <= 0)
            {
                printf("Time quantum must be greater than zero.\n");
                continue;
            }
        }

        if (choice >= 1 && choice <= 7)
        {
            for (i = 0; i < n; i++)
            {
                temp[i] = processes[i];
            }
        }

        switch (choice)
        {
            case 1:
                run_fcfs(temp, n);
                break;

            case 2:
                run_sjf_non_preemptive(temp, n);
                break;

            case 3:
                run_sjf_preemptive(temp, n);
                break;

            case 4:
                run_round_robin(temp, n, quantum);
                break;

            case 5:
                run_priority(temp, n, 0);
                break;

            case 6:
                run_priority(temp, n, 1);
                break;

            case 7:
                printf("\n--- FCFS ---\n");
                run_fcfs(temp, n);

                for (i = 0; i < n; i++)
                    temp[i] = processes[i];
                printf("\n--- SJF Non-Preemptive ---\n");
                run_sjf_non_preemptive(temp, n);

                for (i = 0; i < n; i++)
                    temp[i] = processes[i];
                printf("\n--- SJF Preemptive ---\n");
                run_sjf_preemptive(temp, n);

                for (i = 0; i < n; i++)
                    temp[i] = processes[i];
                printf("\n--- Round Robin ---\n");
                run_round_robin(temp, n, quantum);

                for (i = 0; i < n; i++)
                    temp[i] = processes[i];
                printf("\n--- Priority Non-Preemptive ---\n");
                run_priority(temp, n, 0);

                for (i = 0; i < n; i++)
                    temp[i] = processes[i];
                printf("\n--- Priority Preemptive ---\n");
                run_priority(temp, n, 1);
                break;

            case 0:
                printf("Exiting simulator.\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 0);

    return 0;
}
