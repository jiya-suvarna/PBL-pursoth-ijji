#ifndef THRISHANK_INTEGRATION_H
#define THRISHANK_INTEGRATION_H

#define THRISHANK_MAX_P 50
#define MAX_R 20

typedef struct {
    int processes, resources;
    int total[MAX_R];
    int allocation[THRISHANK_MAX_P][MAX_R];
    int maximum[THRISHANK_MAX_P][MAX_R];
    int need[THRISHANK_MAX_P][MAX_R];
    int available[MAX_R];
} ResourceState;

void prepare_resource_state(
    ResourceState *s,
    int p,
    int r,
    int gui_total[20],
    int gui_allocation[20][20],
    int gui_maximum[20][20]
);

#endif
