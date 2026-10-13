
#include <stdio.h>
#include "schedule.h"

void run_fcfs(Process p[], int n)
{
    int i, j, time = 0;
    int total_burst = 0;
    double total_wt = 0;
    double total_tat = 0;
    double total_rt = 0;

    /* Sort processes by arrival time */
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (p[j].arrival > p[j + 1].arrival)
            {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    printf("\nGantt Chart:\n");

    for (i = 0; i < n; i++)
    {
        if (time < p[i].arrival)
        {
            printf("| Idle ");
            time = p[i].arrival;
        }

        printf("| P%d ", p[i].pid);

        p[i].response = time - p[i].arrival;
        p[i].waiting = time - p[i].arrival;

        time = time + p[i].burst;

        p[i].completion = time;
        p[i].turnaround = p[i].completion - p[i].arrival;

        total_wt = total_wt + p[i].waiting;
        total_tat = total_tat + p[i].turnaround;
        total_rt = total_rt + p[i].response;
        total_burst = total_burst + p[i].burst;
    }

    printf("|\n");

    printf("\nProcess Results:\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\tRT\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].priority,
               p[i].completion,
               p[i].turnaround,
               p[i].waiting,
               p[i].response);
    }

    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time: %.2f", total_tat / n);
    printf("\nAverage Response Time: %.2f", total_rt / n);

    if (time > 0)
    {
        printf("\nCPU Utilization: %.2f%%",
               100.0 * total_burst / time);
    }

    printf("\n");
}
