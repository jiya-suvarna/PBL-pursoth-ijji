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

int main(void) {
    ResourceState s = {0};
    int mode;

    printf("=== Deadlock Simulation: Member 2 ===\n");
    printf("1. Manual input\n2. Random generation\nChoice: ");

    if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2)) {
        printf("Invalid choice.\n");
        return 1;
    }

    printf("Number of processes (1-%d): ", MAX_P);
    if (scanf("%d", &s.processes) != 1) return 1;

    printf("Number of resource types (1-%d): ", MAX_R);
    if (scanf("%d", &s.resources) != 1) return 1;

    if (s.processes < 1 || s.processes > MAX_P ||
        s.resources < 1 || s.resources > MAX_R) {
        printf("Invalid dimensions.\n");
        return 1;
    }

    if (mode == 1) {
        for (int j = 0; j < s.resources; j++) {
            printf("Total instances of R%d: ", j);
            if (scanf("%d", &s.total[j]) != 1) return 1;
        }

        printf("Enter Allocation Matrix row by row:\n");
        for (int i = 0; i < s.processes; i++) {
            printf("P%d: ", i);
            for (int j = 0; j < s.resources; j++)
                if (scanf("%d", &s.allocation[i][j]) != 1)
                    return 1;
        }

        printf("Enter Maximum Matrix row by row:\n");
        for (int i = 0; i < s.processes; i++) {
            printf("P%d: ", i);
            for (int j = 0; j < s.resources; j++)
                if (scanf("%d", &s.maximum[i][j]) != 1)
                    return 1;
        }
    } else {
        for (int j = 0; j < s.resources; j++) {
            s.total[j] = 3 + rand() % 8;
            int remaining = s.total[j];

            for (int i = 0; i < s.processes; i++) {
                int a = remaining > 0
                    ? rand() % (remaining + 1) : 0;
                s.allocation[i][j] = a;
                remaining -= a;
                s.maximum[i][j] =
                    a + rand() % (s.total[j] - a + 1);
            }
        }
    }

    if (!validate(&s)) {
        printf("Invalid data. Check totals, allocations, and maximums.\n");
        return 1;
    }

    calculate(&s);

    print_vector("Total Resources", &s, s.total);
    print_matrix("Allocation Matrix", &s, s.allocation);
    print_matrix("Maximum Matrix", &s, s.maximum);
    print_vector("Available Vector", &s, s.available);
    print_matrix("Need Matrix", &s, s.need);

    printf("\nResource data prepared for Member 3 and Member 4.\n");
    return 0;
}

