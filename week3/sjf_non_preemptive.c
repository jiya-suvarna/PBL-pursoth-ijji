
#include <stdio.h>
#include "schedule.h"

void run_sjf_non_preemptive(Process p[], int n)
{
    int time = 0;
    int completed = 0;
    int done[MAX] = {0};
    int i, idx;
    int total_burst = 0;
    double total_wt = 0;
    double total_tat = 0;
    double total_rt = 0;

    printf("\nGantt Chart:\n");

    while (completed < n)
    {
        idx = -1;

        for (i = 0; i < n; i++)
        {
            if (!done[i] && p[i].arrival <= time)
            {
                if (idx == -1 ||
                    p[i].burst < p[idx].burst ||
                    (p[i].burst == p[idx].burst &&
                     p[i].arrival < p[idx].arrival))
                {
                    idx = i;
                }
            }
        }

        if (idx == -1)
        {
            printf("| Idle (%d-%d) ", time, time + 1);
            time++;
            continue;
        }

        p[idx].response = time - p[idx].arrival;
        p[idx].waiting = p[idx].response;

        printf("| P%d (%d-%d) ", p[idx].pid,
               time, time + p[idx].burst);

        time += p[idx].burst;
        p[idx].completion = time;
        p[idx].turnaround =
            p[idx].completion - p[idx].arrival;

        total_wt += p[idx].waiting;
        total_tat += p[idx].turnaround;
        total_rt += p[idx].response;
        total_burst += p[idx].burst;

        done[idx] = 1;
        completed++;
    }

    printf("|\n\nProcess Results:\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\tRT\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst,
               p[i].priority, p[i].completion,
               p[i].turnaround, p[i].waiting,
               p[i].response);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Response Time: %.2f\n", total_rt / n);

    if (time > 0)
    {
        printf("CPU Utilization: %.2f%%\n",
               100.0 * total_burst / time);
    }
}
