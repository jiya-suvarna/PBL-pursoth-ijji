
#include <stdio.h>

#define MAX_PAGES 100
#define MAX_FRAMES 20

int main(void)
{
    int pages[MAX_PAGES];
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES];

    int n, frameCount;
    int hits = 0, faults = 0;
    int i, j, found, emptyFrame, lruIndex;
    int totalReferences;

    // Step 1: Read the number of page references
    printf("Enter the number of page references: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PAGES) {
        printf("Invalid number of page references!\n");
        return 1;
    }

    // Step 2: Read the page reference string
    printf("Enter the page reference string:\n");
    for (i = 0; i < n; i++) {
        if (scanf("%d", &pages[i]) != 1 || pages[i] < 0) {
            printf("Invalid page number!\n");
            return 1;
        }
    }

    // Step 3: Read the number of frames
    printf("Enter the number of frames: ");
    if (scanf("%d", &frameCount) != 1 ||
        frameCount <= 0 || frameCount > MAX_FRAMES) {
        printf("Invalid number of frames!\n");
        return 1;
    }

    // Step 4: Initialize all frames as empty
    for (i = 0; i < frameCount; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }

    printf("\nLRU PAGE REPLACEMENT SIMULATION\n\n");
    printf("Page\tFrames\t\t\tStatus\n");
    printf("-----------------------------------------------\n");

    // Step 5: Process every page reference
    for (i = 0; i < n; i++) {
        found = 0;
        emptyFrame = -1;

        // Check whether the page is already in memory
        for (j = 0; j < frameCount; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                lastUsed[j] = i;
                break;
            }

            // Find an empty frame
            if (frames[j] == -1 && emptyFrame == -1) {
                emptyFrame = j;
            }
        }

        if (found) {
            // Page hit: the page is already in memory
            hits++;
            printf("%d\t", pages[i]);

            // Print current frame contents
            for (j = 0; j < frameCount; j++) {
                if (frames[j] == -1)
                    printf("- ");
                else
                    printf("%d ", frames[j]);
            }

            printf("\tHIT\n");
        }
        else {
            // Page fault: the page is not in memory
            faults++;

            if (emptyFrame != -1) {
                // Use an empty frame first
                frames[emptyFrame] = pages[i];
                lastUsed[emptyFrame] = i;
            }
            else {
                // All frames are full: find the least
                // recently used page
                lruIndex = 0;

                for (j = 1; j < frameCount; j++) {
                    if (lastUsed[j] < lastUsed[lruIndex]) {
                        lruIndex = j;
                    }
                }

                // Replace the LRU page
                frames[lruIndex] = pages[i];
                lastUsed[lruIndex] = i;
            }

            printf("%d\t", pages[i]);

            // Print updated frame contents
            for (j = 0; j < frameCount; j++) {
                if (frames[j] == -1)
                    printf("- ");
                else
                    printf("%d ", frames[j]);
            }

            printf("\tPAGE FAULT\n");
        }
    }

    // Step 6: Calculate and display statistics
    totalReferences = hits + faults;

    printf("\n========== FINAL STATISTICS ==========\n");
    printf("Total page references : %d\n", totalReferences);
    printf("Number of frames      : %d\n", frameCount);
    printf("Total page hits       : %d\n", hits);
    printf("Total page faults     : %d\n", faults);

    printf("Page hit ratio        : %.2f%%\n",
           (double)hits / totalReferences * 100.0);

    printf("Page fault ratio      : %.2f%%\n",
           (double)faults / totalReferences * 100.0);

    return 0;
}
