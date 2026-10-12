#include <stdio.h>

#define MAX 100

typedef struct {
    int pid;
    int arrival;
    int burst;
    int priority;
    int completion;
    int turnaround;
    int waiting;
    int response;
} Process;

int main(void) {
    Process p[MAX];
    int n, i, time = 0, completed = 0;
    int totalBurst = 0;
    double totalWT = 0, totalTAT = 0, totalRT = 0;
    int done[MAX] = {0};

    printf("Non-Preemptive SJF CPU Scheduling Simulator\n");
    printf("Enter number of processes: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("\nEnter PID, Arrival Time, Burst Time, Priority "
               "for process %d: ", i + 1);

        if (scanf("%d %d %d %d",
                  &p[i].pid, &p[i].arrival,
                  &p[i].burst, &p[i].priority) != 4 ||
            p[i].arrival < 0 || p[i].burst <= 0) {
            printf("Invalid process details.\n");
            return 1;
        }

        for (int j = 0; j < i; j++) {
            if (p[i].pid == p[j].pid) {
                printf("Process IDs must be unique.\n");
                return 1;
            }
        }
    }

    printf("\nGantt Chart:\n");

    while (completed < n) {
        int idx = -1;

        for (i = 0; i < n; i++) {
            if (!done[i] && p[i].arrival <= time) {
                if (idx == -1 ||
                    p[i].burst < p[idx].burst ||
                    (p[i].burst == p[idx].burst &&
                     p[i].arrival < p[idx].arrival)) {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
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

        totalWT += p[idx].waiting;
        totalTAT += p[idx].turnaround;
        totalRT += p[idx].response;
        totalBurst += p[idx].burst;

        done[idx] = 1;
        completed++;
    }

    printf("|\n\nProcess Results:\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\tRT\n");

    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst,
               p[i].priority, p[i].completion,
               p[i].turnaround, p[i].waiting,
               p[i].response);
    }

    printf("\nAverage Waiting Time: %.2f\n", totalWT / n);
    printf("Average Turnaround Time: %.2f\n", totalTAT / n);
    printf("Average Response Time: %.2f\n", totalRT / n);

    if (time > 0) {
        printf("CPU Utilization: %.2f%%\n",
               100.0 * totalBurst / time);
    }

    return 0;
}

