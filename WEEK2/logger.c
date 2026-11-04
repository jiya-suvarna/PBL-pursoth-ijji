#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#define FIFO_NAME "/tmp/simulator_log_fifo"
#define BUFFER_SIZE 1024

// Function to generate timestamp
void get_timestamp(char *buffer, size_t size)
{
    time_t now;
    struct tm *time_info;

    time(&now);
    time_info = localtime(&now);

    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", time_info);
}

// Function to write messages into log files
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

    // Create named pipe
    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            return 1;
        }
    }

    printf("Named Pipe created successfully!\n");
    printf("Waiting for messages from Core...\n");
    printf("FIFO: %s\n\n", FIFO_NAME);

    // Open FIFO for reading
    fifo_fd = open(FIFO_NAME, O_RDONLY);

    if (fifo_fd == -1)
    {
        perror("open FIFO");
        return 1;
    }

    printf("FIFO connected successfully!\n");

    // Continuously receive messages
char line[BUFFER_SIZE];
size_t line_length = 0;
int terminate = 0;

while (!terminate)
{
    ssize_t bytes_read;

    memset(buffer, 0, sizeof(buffer));

    bytes_read = read(fifo_fd, buffer, sizeof(buffer));

    if (bytes_read > 0)
    {
        for (ssize_t i = 0; i < bytes_read; i++)
        {
            if (buffer[i] == '\n')
            {
                line[line_length] = '\0';

                if (strncmp(line, "TERMINATE", 9) == 0)
                {
                    write_log("execution.log", "INFO",
                              "Logger received TERMINATE signal.");

                    printf("Termination signal received.\n");
                    terminate = 1;
                    break;
                }
                else if (strncmp(line, "ERROR|", 6) == 0)
                {
                    write_log("error.log", "ERROR", line + 6);
                    printf("[ERROR] %s\n", line + 6);
                }
                else if (strncmp(line, "INFO:", 5) == 0)
                {
                    write_log("execution.log", "INFO", line + 5);
                    printf("[INFO] %s\n", line + 5);
                }
                else if (line_length > 0)
                {
                    write_log("error.log", "ERROR",
                              "Unknown log message format");

                    printf("[ERROR] Unknown message format\n");
                }

                line_length = 0;
            }
            else if (line_length < sizeof(line) - 1)
            {
                line[line_length++] = buffer[i];
            }
            else
            {
                line_length = 0;
            }
        }
    }
    else if (bytes_read == 0)
    {
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
        if (errno == EINTR)
            continue;

        perror("Error reading FIFO");
        break;
    }
}

    close(fifo_fd);

    // Remove named pipe
    unlink(FIFO_NAME);

    printf("\nLogger process stopped successfully.\n");

    return 0;
}
