
#include <stdio.h>

#define MAX 100
#define MAX_SEGMENTS 100000

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
    int response;
    int first_start;
} Process;

typedef struct {
    int pid;
    int start;
    int end;
} Segment;

Process p[MAX];
Segment gantt[MAX_SEGMENTS];

int queue[MAX + 5];
int front = 0, rear = 0, count = 0;
int n;

// Add a process to the ready queue
void enqueue(int index) {
    queue[rear] = index;
    rear = (rear + 1) % (MAX + 5);
    count++;
}

// Remove a process from the ready queue
int dequeue(void) {
    int index = queue[front];
    front = (front + 1) % (MAX + 5);
    count--;
    return index;
}

// Add newly arrived processes in arrival-time order
void addArrivals(int time, int added[]) {
    while (1) {
        int chosen = -1;

        for (int i = 0; i < n; i++) {
            if (!added[i] && p[i].arrival <= time) {
                if (chosen == -1 ||
                    p[i].arrival < p[chosen].arrival ||
                    (p[i].arrival == p[chosen].arrival &&
                     i < chosen)) {
                    chosen = i;
                }
            }
        }

        if (chosen == -1)
            break;

        enqueue(chosen);
        added[chosen] = 1;
    }
}

int main(void) {
    int quantum;
    int time = 0;
    int completed = 0;
    int segments = 0;
    int added[MAX] = {0};

    double totalWT = 0;
    double totalTAT = 0;
    double totalRT = 0;

    printf("====================================\n");
    printf("   ROUND ROBIN CPU SCHEDULING\n");
    printf("====================================\n");

    printf("Enter number of processes (1-%d): ", MAX);

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    // Get process details
    for (int i = 0; i < n; i++) {
        printf("\nEnter Process ID for process %d: ", i + 1);

        if (scanf("%d", &p[i].pid) != 1) {
            printf("Invalid Process ID.\n");
            return 1;
        }

        // Check for duplicate IDs
        for (int j = 0; j < i; j++) {
            if (p[i].pid == p[j].pid) {
                printf("Process ID must be unique.\n");
                return 1;
            }
        }

        printf("Enter arrival time for P%d: ", p[i].pid);

        if (scanf("%d", &p[i].arrival) != 1 ||
            p[i].arrival < 0) {
            printf("Invalid arrival time.\n");
            return 1;
        }

        printf("Enter burst time for P%d: ", p[i].pid);

        if (scanf("%d", &p[i].burst) != 1 ||
            p[i].burst <= 0) {
            printf("Burst time must be positive.\n");
            return 1;
        }

        p[i].remaining = p[i].burst;
        p[i].completion = 0;
        p[i].turnaround = 0;
        p[i].waiting = 0;
        p[i].response = -1;
        p[i].first_start = -1;
    }

    printf("\nEnter time quantum: ");

    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("Time quantum must be positive.\n");
        return 1;
    }

    // Round Robin scheduling
    while (completed < n) {
        addArrivals(time, added);

        // If ready queue is empty, move to next arrival
        if (count == 0) {
            int next = -1;

            for (int i = 0; i < n; i++) {
                if (!added[i] &&
                    (next == -1 ||
                     p[i].arrival < p[next].arrival ||
                     (p[i].arrival == p[next].arrival &&
                      i < next))) {
                    next = i;
                }
            }

            if (next == -1)
                break;

            time = p[next].arrival;
            addArrivals(time, added);
            continue;
        }

        int i = dequeue();
        int start = time;

        int run = (p[i].remaining < quantum)
                    ? p[i].remaining
                    : quantum;

        // Record the first time this process gets the CPU
        if (p[i].first_start == -1) {
            p[i].first_start = time;
            p[i].response = time - p[i].arrival;
        }

        // Execute process for one time slice
        time += run;
        p[i].remaining -= run;

        if (segments >= MAX_SEGMENTS) {
            printf("Too many Gantt chart segments.\n");
            return 1;
        }

        gantt[segments].pid = p[i].pid;
        gantt[segments].start = start;
        gantt[segments].end = time;
        segments++;

        // Add processes that arrived during this time slice
        addArrivals(time, added);

        if (p[i].remaining == 0) {
            p[i].completion = time;

            p[i].turnaround =
                p[i].completion - p[i].arrival;

            p[i].waiting =
                p[i].turnaround - p[i].burst;

            completed++;
        } else {
            // Unfinished process goes to the back of the queue
            enqueue(i);
        }
    }

    // Display Gantt chart
    printf("\n========== GANTT CHART ==========\n");

    if (segments > 0) {
        for (int i = 0; i < segments; i++) {
            printf("| P%d ", gantt[i].pid);
        }
        printf("|\n");

        printf("%d", gantt[0].start);

        for (int i = 0; i < segments; i++) {
            printf("    %d", gantt[i].end);
        }

        printf("\n");
    }

    // Display process results
    printf("\n========== PROCESS RESULTS ==========\n");

    printf("%-8s %-8s %-8s %-8s %-8s %-8s %-8s\n",
           "PID", "AT", "BT", "CT", "TAT", "WT", "RT");

    for (int i = 0; i < n; i++) {
        printf("%-8d %-8d %-8d %-8d %-8d %-8d %-8d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].completion,
               p[i].turnaround,
               p[i].waiting,
               p[i].response);

        totalWT += p[i].waiting;
        totalTAT += p[i].turnaround;
        totalRT += p[i].response;
    }

    // Display averages
    printf("\n========== AVERAGE METRICS ==========\n");

    printf("Average Waiting Time    : %.2f\n",
           totalWT / n);

    printf("Average Turnaround Time : %.2f\n",
           totalTAT / n);

    printf("Average Response Time   : %.2f\n",
           totalRT / n);

    printf("\nScheduling completed successfully!\n");

    return 0;
}












