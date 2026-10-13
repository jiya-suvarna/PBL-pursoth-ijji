#include <stdio.h>

#define MAX 100
#define MAX_GANTT 100000

typedef struct {
    int pid;
    int arrival;
    int burst;
    int priority;
    int remaining;
    int completion;
    int response;
    int first_start;
} Process;

typedef struct {
    int pid;
    int start;
    int end;
} Gantt;

/* Select the highest-priority process that has arrived.
   Smaller priority number means higher priority. */
int select_process(Process p[], int n, int time) {
    int best = -1;

    for (int i = 0; i < n; i++) {
        if (p[i].arrival > time || p[i].remaining <= 0)
            continue;

        if (best == -1 ||
            p[i].priority < p[best].priority ||
            (p[i].priority == p[best].priority &&
             p[i].arrival < p[best].arrival) ||
            (p[i].priority == p[best].priority &&
             p[i].arrival == p[best].arrival &&
             p[i].pid < p[best].pid)) {
            best = i;
        }
    }

    return best;
}

/* Merge adjacent Gantt chart blocks for the same process. */
void add_gantt(Gantt g[], int *count, int pid,
               int start, int end) {
    if (*count > 0 &&
        g[*count - 1].pid == pid &&
        g[*count - 1].end == start) {
        g[*count - 1].end = end;
    } else if (*count < MAX_GANTT) {
        g[*count].pid = pid;
        g[*count].start = start;
        g[*count].end = end;
        (*count)++;
    }
}

void display_results(Process p[], int n, Gantt g[],
                     int count, const char *title,
                     int finish_time) {
    float total_wait = 0;
    float total_turnaround = 0;
    int total_burst = 0;

    printf("\n===== %s =====\n", title);
    printf("\nPID\tAT\tBT\tPR\tCT\tTAT\tWT\tRT\n");

    for (int i = 0; i < n; i++) {
        int tat = p[i].completion - p[i].arrival;
        int wt = tat - p[i].burst;
        int rt = p[i].first_start - p[i].arrival;

        total_wait += wt;
        total_turnaround += tat;
        total_burst += p[i].burst;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst,
               p[i].priority, p[i].completion,
               tat, wt, rt);
    }

    printf("\nGantt Chart:\n");

    for (int i = 0; i < count; i++) {
        if (g[i].pid == -1)
            printf("| Idle ");
        else
            printf("| P%d ", g[i].pid);
    }
    printf("|\n");

    if (count > 0) {
        printf("%d", g[0].start);

        for (int i = 0; i < count; i++)
            printf("\t%d", g[i].end);

        printf("\n");
    }

    printf("\nAverage Waiting Time: %.2f\n",
           total_wait / n);

    printf("Average Turnaround Time: %.2f\n",
           total_turnaround / n);

    if (finish_time > 0) {
        printf("CPU Utilization: %.2f%%\n",
               total_burst * 100.0 / finish_time);
    }
}

/* Run either non-preemptive or preemptive priority scheduling. */
void run_priority(Process input[], int n, int preemptive) {
    Process p[MAX];
    Gantt g[MAX_GANTT];

    int count = 0;
    int completed = 0;
    int time = 0;

    for (int i = 0; i < n; i++) {
        p[i] = input[i];
        p[i].remaining = p[i].burst;
        p[i].completion = 0;
        p[i].first_start = -1;
    }

    while (completed < n) {
        int best = select_process(p, n, time);

        if (best == -1) {
            add_gantt(g, &count, -1, time, time + 1);
            time++;
            continue;
        }

        if (p[best].first_start == -1)
            p[best].first_start = time;

        if (preemptive) {
            add_gantt(g, &count, p[best].pid,
                      time, time + 1);

            p[best].remaining--;
            time++;

            if (p[best].remaining == 0) {
                p[best].completion = time;
                completed++;
            }
        } else {
            int start = time;

            time += p[best].remaining;
            p[best].remaining = 0;
            p[best].completion = time;
            completed++;

            add_gantt(g, &count, p[best].pid,
                      start, time);
        }
    }

    display_results(
        p, n, g, count,
        preemptive ? "PREEMPTIVE PRIORITY SCHEDULING"
                   : "NON-PREEMPTIVE PRIORITY SCHEDULING",
        time
    );
}

int main(void) {
    Process p[MAX];
    int n, choice;

    printf("====================================\n");
    printf("   PRIORITY SCHEDULING SIMULATOR\n");
    printf("====================================\n");
    printf("Smaller priority number = Higher priority\n");

    printf("\nEnter number of processes (1-%d): ", MAX);

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);

        printf("Enter Process ID: ");
        if (scanf("%d", &p[i].pid) != 1) {
            printf("Invalid process ID.\n");
            return 1;
        }

        printf("Enter Arrival Time: ");
        if (scanf("%d", &p[i].arrival) != 1 ||
            p[i].arrival < 0) {
            printf("Invalid arrival time.\n");
            return 1;
        }

        printf("Enter Burst Time: ");
        if (scanf("%d", &p[i].burst) != 1 ||
            p[i].burst <= 0) {
            printf("Burst time must be positive.\n");
            return 1;
        }

        printf("Enter Priority: ");
        if (scanf("%d", &p[i].priority) != 1 ||
            p[i].priority < 0) {
            printf("Priority must be non-negative.\n");
            return 1;
        }
    }

    do {
        printf("\n========== MENU ==========\n");
        printf("1. Non-Preemptive Priority\n");
        printf("2. Preemptive Priority\n");
        printf("3. Run Both Algorithms\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid menu input.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                run_priority(p, n, 0);
                break;

            case 2:
                run_priority(p, n, 1);
                break;

            case 3:
                run_priority(p, n, 0);
                run_priority(p, n, 1);
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