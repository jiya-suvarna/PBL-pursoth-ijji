
#include <stdio.h>
#include "schedule.h"

void input_processes(Process p[], int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        printf("\nEnter details for Process %d\n", i + 1);

        p[i].pid = i + 1;

        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].arrival);

        printf("Enter Burst Time: ");
        scanf("%d", &p[i].burst);

        printf("Enter Priority: ");
        scanf("%d", &p[i].priority);

        p[i].remaining = p[i].burst;
        p[i].completion = 0;
        p[i].turnaround = 0;
        p[i].waiting = 0;
        p[i].response = -1;
        p[i].started = 0;
    }
}

void display_menu(void)
{
    printf("\n===== CPU SCHEDULING SIMULATOR =====\n");
    printf("1. FCFS\n");
    printf("2. SJF Non-Preemptive\n");
    printf("3. SJF Preemptive (SRTF)\n");
    printf("4. Round Robin\n");
    printf("5. Priority Non-Preemptive\n");
    printf("6. Priority Preemptive\n");
    printf("7. Run All Algorithms\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}
