
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

typedef struct {
    double avg_wt;
    double avg_tat;
    double avg_rt;
    double cpu_util;
} Metrics;

void input_processes(Process p[], int n);
void display_menu(void);

void run_fcfs(Process p[], int n);
void run_sjf_non_preemptive(Process p[], int n);
void run_sjf_preemptive(Process p[], int n);
void run_round_robin(Process p[], int n, int quantum);
void run_priority(Process p[], int n, int preemptive);

void run_fcfs_compare(Process p[], int n, Metrics *m);
void run_sjf_non_preemptive_compare(Process p[], int n, Metrics *m);
void run_sjf_preemptive_compare(Process p[], int n, Metrics *m);
void run_round_robin_compare(Process p[], int n, int quantum, Metrics *m);
void run_priority_compare(Process p[], int n, int preemptive, Metrics *m);

#endif
