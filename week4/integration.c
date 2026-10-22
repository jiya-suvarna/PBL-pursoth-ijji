#include "banker_integration.h"

/* These are Sayam's original Banker arrays */
extern int allocation[MAX_P][MAX_R];
extern int maximum[MAX_P][MAX_R];
extern int available[MAX_R];

int run_banker_from_gui(
    int p,
    int r,
    int gui_allocation[20][20],
    int gui_maximum[20][20],
    int gui_available[20],
    int safeSequence[20]
)
{
    int i, j;

    processes = p;
    resources = r;

    /* Copy GUI data into Sayam's Banker arrays */
    for (i = 0; i < p; i++)
    {
        for (j = 0; j < r; j++)
        {
            allocation[i][j] = gui_allocation[i][j];
            maximum[i][j] = gui_maximum[i][j];
        }
    }

    for (j = 0; j < r; j++)
        available[j] = gui_available[j];

    /* Use Sayam's original functions */
    calculateNeed();

    return safetyCheck(safeSequence);
}