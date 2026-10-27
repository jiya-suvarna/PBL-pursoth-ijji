#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define MAX_PAGES 1000
#define MAX_FRAMES 100
#define INPUT_SIZE 20000

/*
 * Member 2: FIFO Page Replacement Algorithm
 *
 * Reusable function:
 * fifo_simulate(pages, page_count, frame_count)
 *
 * This function can be called by the team leader
 * during integration with the other algorithms and GUI.
 */

void fifo_simulate(const int pages[], int page_count, int frame_count)
{
    int frames[MAX_FRAMES];
    int hits = 0;
    int faults = 0;
    int next_to_replace = 0;

    if (pages == NULL || page_count <= 0 ||
        frame_count <= 0 || frame_count > MAX_FRAMES)
    {
        printf("Error: Invalid page references or frame count.\n");
        return;
    }

    /* Initialize all frames as empty. */
    for (int i = 0; i < frame_count; i++)
    {
        frames[i] = -1;
    }

    printf("\nFIFO Page Replacement Simulation\n\n");
    printf("%-10s", "Page");

    for (int i = 0; i < frame_count; i++)
    {
        char label[20];
        snprintf(label, sizeof(label), "Frame %d", i + 1);
        printf("%-12s", label);
    }

    printf("%-10s\n", "Status");

    /* Process every page request. */
    for (int i = 0; i < page_count; i++)
    {
        int current_page = pages[i];
        int found = 0;
        int empty_frame = -1;

        /* Check whether the requested page is already present. */
        for (int j = 0; j < frame_count; j++)
        {
            if (frames[j] == current_page)
            {
                found = 1;
                break;
            }

            if (frames[j] == -1 && empty_frame == -1)
            {
                empty_frame = j;
            }
        }

        if (found)
        {
            hits++;
        }
        else
        {
            faults++;

            if (empty_frame != -1)
            {
                /* Fill an empty frame first. */
                frames[empty_frame] = current_page;
            }
            else
            {
                /* Replace the oldest page using FIFO. */
                frames[next_to_replace] = current_page;

                next_to_replace =
                    (next_to_replace + 1) % frame_count;
            }
        }

        /* Display the frame contents after each request. */
        printf("%-10d", current_page);

        for (int j = 0; j < frame_count; j++)
        {
            if (frames[j] == -1)
            {
                printf("%-12s", "-");
            }
            else
            {
                printf("%-12d", frames[j]);
            }
        }

        printf("%-10s\n", found ? "Hit" : "Fault");
    }

    /* Calculate hit and fault percentages. */
    double hit_ratio = (double)hits / page_count * 100.0;
    double fault_ratio = (double)faults / page_count * 100.0;

    printf("\n========== FIFO SUMMARY ==========\n");
    printf("Total page references : %d\n", page_count);
    printf("Total page hits       : %d\n", hits);
    printf("Total page faults     : %d\n", faults);
    printf("Page hit ratio        : %.2f%%\n", hit_ratio);
    printf("Page fault ratio      : %.2f%%\n", fault_ratio);
    printf("==================================\n");
}

/*
 * Standalone test program.
 *
 * The team leader can compile this module without the
 * test main function by defining FIFO_MODULE_ONLY.
 */

#ifndef FIFO_MODULE_ONLY

int main(void)
{
    int pages[MAX_PAGES];
    int page_count = 0;
    int frame_count;

    char input[INPUT_SIZE];
    char *token;

    printf("====================================\n");
    printf("   FIFO PAGE REPLACEMENT SIMULATOR\n");
    printf("====================================\n");

    printf("Enter page-reference string:\n");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        printf("Error: Could not read the page-reference string.\n");
        return 1;
    }

    token = strtok(input, " \t\r\n");

    while (token != NULL)
    {
        char *end;
        long value;

        errno = 0;
        value = strtol(token, &end, 10);

        if (end == token || *end != '\0' ||
            errno == ERANGE || value < 0 || value > INT_MAX)
        {
            printf("Error: Enter non-negative integer page numbers only.\n");
            return 1;
        }

        if (page_count >= MAX_PAGES)
        {
            printf("Error: Too many page references. Maximum is %d.\n",
                   MAX_PAGES);
            return 1;
        }

        pages[page_count++] = (int)value;
        token = strtok(NULL, " \t\r\n");
    }

    if (page_count == 0)
    {
        printf("Error: The page-reference string cannot be empty.\n");
        return 1;
    }

    printf("Enter number of page frames: ");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        printf("Error: Could not read the frame count.\n");
        return 1;
    }

    char *end;
    errno = 0;
    long frame_value = strtol(input, &end, 10);

    while (*end == ' ' || *end == '\t' ||
           *end == '\r' || *end == '\n')
    {
        end++;
    }

    if (end == input || *end != '\0' ||
        errno == ERANGE ||
        frame_value <= 0 || frame_value > MAX_FRAMES)
    {
        printf("Error: Frame count must be between 1 and %d.\n",
               MAX_FRAMES);
        return 1;
    }

    frame_count = (int)frame_value;

    fifo_simulate(pages, page_count, frame_count);

    return 0;
}

#endif
