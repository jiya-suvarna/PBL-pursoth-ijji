
#include <stdio.h>
#include "schedule.h"

void run_round_robin(Process p[], int n, int quantum)
{
    int queue[MAX + 1];
    int added[MAX] = {0};
    int front = 0, rear = 0, count = 0;
    int time = 0, completed = 0;
    int i, idx, run, next;
    int total_burst = 0;
    double total_wt = 0, total_tat = 0, total_rt = 0;

    if (quantum <= 0)
    {
        printf("Time quantum must be positive.\n");
        return;
    }

    for (i = 0; i < n; i++)
    {
        p[i].remaining = p[i].burst;
        p[i].completion = 0;
        p[i].turnaround = 0;
        p[i].waiting = 0;
        p[i].response = -1;
        p[i].started = 0;
        total_burst += p[i].burst;
    }

    printf("\nRound Robin Scheduling (Quantum = %d)\n", quantum);
    printf("\nGantt Chart:\n");

    while (completed < n)
    {
        /* Add processes that have arrived */
        while (1)
        {
            idx = -1;

            for (i = 0; i < n; i++)
            {
                if (!added[i] && p[i].arrival <= time)
                {
                    if (idx == -1 ||
                        p[i].arrival < p[idx].arrival ||
                        (p[i].arrival == p[idx].arrival &&
                         p[i].pid < p[idx].pid))
                    {
                        idx = i;
                    }
                }
            }

            if (idx == -1)
                break;

            queue[rear] = idx;
            rear = (rear + 1) % (MAX + 1);
            count++;
            added[idx] = 1;
        }

        /* Handle CPU idle time */
        if (count == 0)
        {
            next = -1;

            for (i = 0; i < n; i++)
            {
                if (!added[i] &&
                    (next == -1 ||
                     p[i].arrival < p[next].arrival ||
                     (p[i].arrival == p[next].arrival &&
                      p[i].pid < p[next].pid)))
                {
                    next = i;
                }
            }

            if (next == -1)
                break;

            if (time < p[next].arrival)
            {
                printf("| Idle (%d-%d) ",
                       time, p[next].arrival);
                time = p[next].arrival;
            }

            continue;
        }

        /* Select the next process */
        idx = queue[front];
        front = (front + 1) % (MAX + 1);
        count--;

        if (!p[idx].started)
        {
            p[idx].response = time - p[idx].arrival;
            p[idx].started = 1;
        }

        run = p[idx].remaining;

        if (run > quantum)
            run = quantum;

        printf("| P%d (%d-%d) ",
               p[idx].pid, time, time + run);

        time += run;
        p[idx].remaining -= run;

        /* Add newly arrived processes to the queue */
        while (1)
        {
            next = -1;

            for (i = 0; i < n; i++)
            {
                if (!added[i] && p[i].arrival <= time)
                {
                    if (next == -1 ||
                        p[i].arrival < p[next].arrival ||
                        (p[i].arrival == p[next].arrival &&
                         p[i].pid < p[next].pid))
                    {
                        next = i;
                    }
                }
            }

            if (next == -1)
                break;

            queue[rear] = next;
            rear = (rear + 1) % (MAX + 1);
            count++;
            added[next] = 1;
        }

        if (p[idx].remaining == 0)
        {
            p[idx].completion = time;
            p[idx].turnaround =
                p[idx].completion - p[idx].arrival;
            p[idx].waiting =
                p[idx].turnaround - p[idx].burst;
            completed++;
        }
        else
        {
            queue[rear] = idx;
            rear = (rear + 1) % (MAX + 1);
            count++;
        }
    }

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
