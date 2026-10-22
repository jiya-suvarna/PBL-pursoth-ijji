#ifndef BANKER_INTEGRATION_H
#define BANKER_INTEGRATION_H

#define MAX_P 20
#define MAX_R 20

extern int processes;
extern int resources;

void calculateNeed(void);
int safetyCheck(int safeSequence[MAX_P]);


int run_banker_from_gui(
    int p,
    int r,
    int gui_allocation[20][20],
    int gui_maximum[20][20],
    int gui_available[20],
    int safeSequence[20]
);
#endif
