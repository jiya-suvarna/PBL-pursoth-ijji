#ifndef DIGANTH_INTEGRATION_H
#define DIGANTH_INTEGRATION_H

#ifdef __cplusplus
extern "C" {
#endif

int run_deadlock_detection_from_gui(
    int p,
    int r,
    int allocation[20][20],
    int request[20][20],
    int available[20],
    int deadlockedProcesses[20],
    int completionOrder[20],
    int *deadlockedCount,
    int *completionCount
);

int run_deadlock_recovery_from_gui(
    int p,
    int r,
    int allocation[20][20],
    int need[20][20],
    int available[20],
    int deadlockedProcess
);

int run_rag_from_gui(
    int p,
    int r,
    int allocation[20][20],
    int request[20][20],
    int deadlockedProcesses[20],
    int deadlockedCount
);

#ifdef __cplusplus
}
#endif

#endif
