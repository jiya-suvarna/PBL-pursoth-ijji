#include <stdio.h>

#define MAX_PAGES 100
#define MAX_FRAMES 20

void optimal_simulate(const int pages[], int n, int frame_count, int *hits_out, int *faults_out)
{
    int frames[MAX_FRAMES];
    int hits = 0, faults = 0;


    for (int i = 0; i < frame_count; i++)
        frames[i] = -1;

    printf("\nPage\t");
    for (int i = 0; i < frame_count; i++)
        printf("F%d\t", i + 1);
    printf("Status\n");

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == page) {
                found = 1;
                break;
            }
        }

        if (found) {
            hits++;
        } else {
            faults++;
            int replace = -1;

            for (int j = 0; j < frame_count; j++) {
                if (frames[j] == -1) {
                    replace = j;
                    break;
                }
            }

            if (replace == -1) {
                int farthest = -1;

                for (int j = 0; j < frame_count; j++) {
                    int next = -1;

                    for (int k = i + 1; k < n; k++) {
                        if (pages[k] == frames[j]) {
                            next = k;
                            break;
                        }
                    }

                    if (next == -1) {
                        replace = j;
                        break;
                    }

                    if (next > farthest) {
                        farthest = next;
                        replace = j;
                    }
                }
            }

            frames[replace] = page;
        }

        printf("%d\t", page);
        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == -1)
                printf("-\t");
            else
                printf("%d\t", frames[j]);
        }
        printf("%s\n", found ? "HIT" : "FAULT");
    }

    printf("\nTotal page hits: %d\n", hits);
    printf("Total page faults: %d\n", faults);
    printf("Hit ratio: %.2f%%\n", (double)hits / n * 100);
    printf("Fault ratio: %.2f%%\n", (double)faults / n * 100);

    *hits_out = hits;
    *faults_out = faults;

}
