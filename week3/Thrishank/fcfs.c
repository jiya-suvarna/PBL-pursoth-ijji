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
    int n, i, j, time = 0;
    double totalWT = 0, totalTAT = 0, totalRT = 0;
    int totalBurst = 0;

    printf("FCFS CPU Scheduling Simulator\n");
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

        for (j = 0; j < i; j++) {
            if (p[i].pid == p[j].pid) {
                printf("Process IDs must be unique.\n");
                return 1;
            }
        }
    }

    /* Sort by arrival time; preserve input order for ties. */
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].arrival > p[j + 1].arrival) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    printf("\nGantt Chart:\n");
    printf("0");

    for (i = 0; i < n; i++) {
        /* CPU stays idle until the next process arrives. */
        if (time < p[i].arrival) {
            time = p[i].arrival;
        }

        p[i].response = time - p[i].arrival;
        p[i].waiting = time - p[i].arrival;

        time += p[i].burst;
        p[i].completion = time;

        p[i].turnaround =
            p[i].completion - p[i].arrival;

        totalWT += p[i].waiting;
        totalTAT += p[i].turnaround;
        totalRT += p[i].response;
        totalBurst += p[i].burst;

        printf(" -- P%d -- %d", p[i].pid, time);
    }

    printf("\n\nProcess Results:\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\tRT\n");

    for (i = 0; i < n; i++) {
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

    printf("\nAverage Waiting Time: %.2f",
           totalWT / n);
    printf("\nAverage Turnaround Time: %.2f",
           totalTAT / n);
    printf("\nAverage Response Time: %.2f",
           totalRT / n);

    if (time > 0) {
        printf("\nCPU Utilization: %.2f%%",
               100.0 * totalBurst / time);
    }

    printf("\n");
    return 0;
}
