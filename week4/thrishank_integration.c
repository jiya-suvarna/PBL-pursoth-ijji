#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_P 50
#define MAX_R 20

typedef struct {
    int processes, resources;
    int total[MAX_R];
    int allocation[MAX_P][MAX_R];
    int maximum[MAX_P][MAX_R];
    int need[MAX_P][MAX_R];
    int available[MAX_R];
} ResourceState;

bool validate(ResourceState *s) {
    if (s->processes < 1 || s->processes > MAX_P ||
        s->resources < 1 || s->resources > MAX_R)
        return false;

    for (int j = 0; j < s->resources; j++) {
        if (s->total[j] < 0) return false;
        int allocated = 0;

        for (int i = 0; i < s->processes; i++) {
            int a = s->allocation[i][j];
            int m = s->maximum[i][j];

            if (a < 0 || m < 0 || a > m) return false;
            allocated += a;
        }

        if (allocated > s->total[j]) return false;
    }
    return true;
}

void calculate(ResourceState *s) {
    for (int j = 0; j < s->resources; j++) {
        s->available[j] = s->total[j];

        for (int i = 0; i < s->processes; i++)
            s->available[j] -= s->allocation[i][j];
    }

    for (int i = 0; i < s->processes; i++)
        for (int j = 0; j < s->resources; j++)
            s->need[i][j] =
                s->maximum[i][j] - s->allocation[i][j];
}

void print_vector(const char *title, ResourceState *s, int v[]) {
    printf("\n%s: ", title);
    for (int j = 0; j < s->resources; j++)
        printf("%d ", v[j]);
    printf("\n");
}

void print_matrix(const char *title, ResourceState *s,
                  int matrix[MAX_P][MAX_R]) {
    printf("\n%s\n", title);
    printf("     ");
    for (int j = 0; j < s->resources; j++)
        printf("R%d ", j);
    printf("\n");

    for (int i = 0; i < s->processes; i++) {
        printf("P%d   ", i);
        for (int j = 0; j < s->resources; j++)
            printf("%d  ", matrix[i][j]);
        printf("\n");
    }
}
void prepare_resource_state(
    ResourceState *s,
    int p,
    int r,
    int gui_total[20],
    int gui_allocation[20][20],
    int gui_maximum[20][20]
)
{
    int i, j;

    s->processes = p;
    s->resources = r;

    for (j = 0; j < r; j++)
        s->total[j] = gui_total[j];

    for (i = 0; i < p; i++)
    {
        for (j = 0; j < r; j++)
        {
            s->allocation[i][j] = gui_allocation[i][j];
            s->maximum[i][j] = gui_maximum[i][j];
        }
    }

    calculate(s);
}
