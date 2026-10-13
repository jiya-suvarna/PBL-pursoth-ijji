
#include <stdio.h>
#include "schedule.h"

void run_sjf_preemptive(Process p[], int n)
{
    int time = 0, completed = 0;
    int i, idx;
    int total_burst = 0;
    int last_pid = -1;
    int segment_start = 0;
    double total_wt = 0, total_tat = 0, total_rt = 0;

    for (i = 0; i < n; i++)
    {
        p[i].remaining = p[i].burst;
        p[i].started = 0;
        p[i].response = -1;
        total_burst += p[i].burst;
    }

    printf("\nGantt Chart:\n");

    while (completed < n)
    {
        idx = -1;

        for (i = 0; i < n; i++)
        {
            if (p[i].arrival <= time && p[i].remaining > 0)
            {
                if (idx == -1 ||
                    p[i].remaining < p[idx].remaining ||
                    (p[i].remaining == p[idx].remaining &&
                     p[i].arrival < p[idx].arrival))
                {
                    idx = i;
                }
            }
        }

        if (idx == -1)
        {
            if (last_pid != -2)
            {
                if (last_pid > 0)
                    printf("| P%d ", last_pid);

                last_pid = -2;
            }

            time++;
            continue;
        }

        if (last_pid != p[idx].pid)
        {
            if (last_pid > 0)
                printf("| P%d ", last_pid);
            else if (last_pid == -2)
                printf("| Idle ");

            last_pid = p[idx].pid;
        }

        if (!p[idx].started)
        {
            p[idx].response = time - p[idx].arrival;
            p[idx].started = 1;
        }

        p[idx].remaining--;
        time++;

        if (p[idx].remaining == 0)
        {
            p[idx].completion = time;
            p[idx].turnaround =
                p[idx].completion - p[idx].arrival;
            p[idx].waiting =
                p[idx].turnaround - p[idx].burst;
            completed++;
        }
    }

    if (last_pid > 0)
        printf("| P%d ", last_pid);
    else if (last_pid == -2)
        printf("| Idle ");

    printf("|\n\nProcess Results:\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\tRT\n");

    for (i = 0; i < n; i++)
    {
        total_wt += p[i].waiting;
        total_tat += p[i].turnaround;
        total_rt += p[i].response;

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
        printf("CPU Utilization: %.2f%%\n",
               total_burst * 100.0 / time);
}


