#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/simulator_log_fifo"

int main()
{
    int fd;
    char message[1024];

    printf("\n==============================\n");
    printf("      CORE PROCESS STARTED\n");
    printf("==============================\n");

    printf("Connecting to Logger...\n");

    /* Open FIFO for writing */
    fd = open(FIFO_NAME, O_WRONLY);

    if (fd == -1)
    {
        perror("Error opening FIFO");
        return 1;
    }

    printf("Connected to Logger!\n\n");

    /* Send first message */
    strcpy(message, "INFO: Core process started successfully");
    write(fd, message, strlen(message) + 1);

    sleep(2);

    /* Send second message */
    strcpy(message, "INFO: CPU and Memory module initialized");
    write(fd, message, strlen(message) + 1);

    sleep(2);

    /* Send third message */
    strcpy(message, "INFO: Stack and Queue module initialized");
    write(fd, message, strlen(message) + 1);

    sleep(2);

    /* Send fourth message */
    strcpy(message, "WARNING: Testing logger communication");
    write(fd, message, strlen(message) + 1);

    sleep(2);

    /* Send final message */
    strcpy(message, "INFO: Core process completed");
    write(fd, message, strlen(message) + 1);

    close(fd);

    printf("\nMessages sent successfully!\n");
    printf("Core process finished.\n");

    return 0;
}
