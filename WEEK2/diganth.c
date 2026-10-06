#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#include "ipc.h"   // IPC definitions

void get_timestamp(char *buffer, size_t size)
{
    time_t now;
    struct tm *time_info;

    time(&now);
    time_info = localtime(&now);

    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", time_info);
}

void write_log(const char *filename,
               const char *level,
               const char *message)
{
    FILE *file;
    char timestamp[64];

    get_timestamp(timestamp, sizeof(timestamp));

    file = fopen(filename, "a");

    if (file == NULL)
    {
        perror("Unable to open log file");
        return;
    }

    fprintf(file,
            "[%s] [%s] [Logger PID: %d] %s\n",
            timestamp,
            level,
            getpid(),
            message);

    fclose(file);
}

int main()
{
    int fifo_fd;
    char buffer[BUFFER_SIZE];

    printf("=====================================\n");
    printf("       LOGGER PROCESS STARTED        \n");
    printf("=====================================\n");

    /*
     * Create FIFO if it does not already exist
     */
    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            return 1;
        }
    }

    printf("Waiting for messages from Core...\n");
    printf("FIFO: %s\n\n", FIFO_NAME);

    /*
     * Open FIFO for reading
     */
    fifo_fd = open(FIFO_NAME, O_RDONLY);

    if (fifo_fd == -1)
    {
        perror("open FIFO");
        return 1;
    }

    /*
     * Continuously receive messages
     */
    while (1)
    {
        ssize_t bytes_read;

        memset(buffer, 0, sizeof(buffer));

        bytes_read = read(fifo_fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';

            /*
             * TERMINATE message stops Logger
             */
            if (strncmp(buffer, "TERMINATE", 9) == 0)
            {
                write_log(
                    "execution.log",
                    "INFO",
                    "Logger received TERMINATE signal."
                );

                printf("Termination signal received.\n");
                break;
            }

            /*
             * ERROR messages go to error.log
             */
            if (strncmp(buffer, "ERROR|", 6) == 0)
            {
                write_log(
                    "error.log",
                    "ERROR",
                    buffer + 6
                );

                printf("[ERROR] %s\n", buffer + 6);
            }

            /*
             * INFO messages go to execution.log
             */
            else if (strncmp(buffer, "INFO|", 5) == 0)
            {
                write_log(
                    "execution.log",
                    "INFO",
                    buffer + 5
                );

                printf("[INFO] %s\n", buffer + 5);
            }

            /*
             * Unknown messages are treated as errors
             */
            else
            {
                write_log(
                    "error.log",
                    "ERROR",
                    "Unknown log message format"
                );

                printf("[ERROR] Unknown message format\n");
            }
        }

        else if (bytes_read == 0)
        {
            /*
             * Writer closed the FIFO.
             * Reopen it so Logger can continue waiting.
             */

            close(fifo_fd);

            fifo_fd = open(FIFO_NAME, O_RDONLY);

            if (fifo_fd == -1)
            {
                perror("Reopening FIFO");
                break;
            }
        }

        else
        {
            perror("Error reading FIFO");
            break;
        }
    }

    close(fifo_fd);

    /*
     * Remove FIFO after Logger finishes
     */
    unlink(FIFO_NAME);

    printf("\nLogger process stopped successfully.\n");

    return 0;
}