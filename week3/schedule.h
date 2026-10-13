
#ifndef SCHEDULE_H
#define SCHEDULE_H

#define MAX 100
#define MAX_SEGMENTS 1000

typedef struct {
    int pid;
    int arrival;
    int burst;
    int priority;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
    int response;
    int started;
} Process;

void input_processes(Process p[], int n);
void display_menu(void);

void run_fcfs(Process p[], int n);
void run_sjf_non_preemptive(Process p[], int n);
void run_sjf_preemptive(Process p[], int n);
void run_round_robin(Process p[], int n, int quantum);
void run_priority(Process p[], int n, int preemptive);

#endif
