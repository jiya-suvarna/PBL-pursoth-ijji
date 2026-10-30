#include <stdio.h>

#define MAX_PAGES 100
#define MAX_FRAMES 20

void fifo_simulate(const int pages[], int page_count, int frame_count, int *hits_out, int *faults_out);
void lru_simulate(const int pages[], int n, int frameCount, int *hits_out, int *faults_out);
void optimal_simulate(const int pages[], int n, int frame_count, int *hits_out, int *faults_out);


void compare_algorithms(const int pages[], int n, int frame_count)
{
    int fifo_hits, fifo_faults;
    int lru_hits, lru_faults;
    int optimal_hits, optimal_faults;

    fifo_simulate(pages, n, frame_count, &fifo_hits, &fifo_faults);
    lru_simulate(pages, n, frame_count, &lru_hits, &lru_faults);
    optimal_simulate(pages, n, frame_count, &optimal_hits, &optimal_faults);

    printf("\n========== ALGORITHM COMPARISON ==========\n");
    printf("Algorithm    Hits    Faults    Hit Ratio    Fault Ratio\n");

    printf("FIFO         %d      %d        %.2f%%       %.2f%%\n",
           fifo_hits, fifo_faults,
           (double)fifo_hits / n * 100.0,
           (double)fifo_faults / n * 100.0);

    printf("LRU          %d      %d        %.2f%%       %.2f%%\n",
           lru_hits, lru_faults,
           (double)lru_hits / n * 100.0,
           (double)lru_faults / n * 100.0);

    printf("Optimal      %d      %d        %.2f%%       %.2f%%\n",
           optimal_hits, optimal_faults,
           (double)optimal_hits / n * 100.0,
           (double)optimal_faults / n * 100.0);

    printf("===========================================\n");

}

