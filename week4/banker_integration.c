#include <stdio.h>
#include "banker_integration.h"


#define MAX_P 20
#define MAX_R 20

int allocation[MAX_P][MAX_R];
int maximum[MAX_P][MAX_R];
int need[MAX_P][MAX_R];
int available[MAX_R];

int processes, resources;

/* Calculate Need = Maximum - Allocation */
void calculateNeed(void)
{
    for (int i = 0; i < processes; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

/* Display a matrix */
void displayMatrix(const char *title, int matrix[MAX_P][MAX_R])
{
    printf("\n%s\n", title);
    printf("       ");

    for (int j = 0; j < resources; j++)
        printf("R%-6d", j);

    printf("\n");

    for (int i = 0; i < processes; i++)
    {
        printf("P%-6d", i);

        for (int j = 0; j < resources; j++)
            printf("%-7d", matrix[i][j]);

        printf("\n");
    }
}

/* Display Available resources */
void displayAvailable(void)
{
    printf("\nAvailable Vector: ");

    for (int j = 0; j < resources; j++)
        printf("R%d=%d ", j, available[j]);

    printf("\n");
}

/* Banker's safety algorithm */
int safetyCheck(int safeSequence[MAX_P])
{
    int work[MAX_R];
    int finish[MAX_P] = {0};

    for (int j = 0; j < resources; j++)
        work[j] = available[j];

    int completed = 0;

    while (completed < processes)
    {
        int found = 0;

        for (int i = 0; i < processes; i++)
        {
            if (finish[i])
                continue;

            int canFinish = 1;

            for (int j = 0; j < resources; j++)
            {
                if (need[i][j] > work[j])
                {
                    canFinish = 0;
                    break;
                }
            }

            if (canFinish)
            {
                for (int j = 0; j < resources; j++)
                    work[j] += allocation[i][j];

                safeSequence[completed] = i;
                finish[i] = 1;
                completed++;
                found = 1;
            }
        }

        if (!found)
            return 0;
    }

    return 1;
}

/* Check a process's resource request */
void requestResources(void)
{
    int pid;
    int request[MAX_R];
    int safeSequence[MAX_P];

    printf("\nEnter process ID (0 to %d): ", processes - 1);
    scanf("%d", &pid);

    if (pid < 0 || pid >= processes)
    {
        printf("Invalid process ID!\n");
        return;
    }

    printf("Enter request for each resource (R0 to R%d): ",
           resources - 1);

    for (int j = 0; j < resources; j++)
    {
        if (scanf("%d", &request[j]) != 1 || request[j] < 0)
        {
            printf("Invalid request!\n");
            return;
        }
    }

    /* Request must not exceed the process's remaining need */
    for (int j = 0; j < resources; j++)
    {
        if (request[j] > need[pid][j])
        {
            printf("Request DENIED: exceeds process need.\n");
            return;
        }
    }

    /* Requested resources must be available */
    for (int j = 0; j < resources; j++)
    {
        if (request[j] > available[j])
        {
            printf("Request cannot be granted now: "
                   "resources are unavailable.\n");
            return;
        }
    }

    /* Tentatively allocate the requested resources */
    for (int j = 0; j < resources; j++)
    {
        available[j] -= request[j];
        allocation[pid][j] += request[j];
        need[pid][j] -= request[j];
    }

    /* Grant only if the resulting state is safe */
    if (safetyCheck(safeSequence))
    {
        printf("\nREQUEST GRANTED: system remains SAFE.\n");

        printf("Safe Sequence: ");

        for (int i = 0; i < processes; i++)
        {
            printf("P%d", safeSequence[i]);

            if (i < processes - 1)
                printf(" -> ");
        }

        printf("\n");
    }
    else
    {
        /* Roll back the tentative allocation */
        for (int j = 0; j < resources; j++)
        {
            available[j] += request[j];
            allocation[pid][j] -= request[j];
            need[pid][j] += request[j];
        }

        printf("\nREQUEST DENIED: unsafe state detected.\n");
        printf("Allocation has been rolled back.\n");
    }
}

/* Main program */
/* GUI wrapper for Banker's Algorithm */
int run_banker_from_gui(
    int p,
    int r,
    int gui_allocation[20][20],
    int gui_maximum[20][20],
    int gui_available[20],
    int safeSequence[20]
)
{
    processes = p;
    resources = r;

    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            allocation[i][j] = gui_allocation[i][j];
            maximum[i][j] = gui_maximum[i][j];
        }
    }

    for (int j = 0; j < r; j++)
        available[j] = gui_available[j];

    calculateNeed();

    return safetyCheck(safeSequence);
}