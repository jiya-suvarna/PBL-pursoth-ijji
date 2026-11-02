#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_INSTRUCTIONS 100
#define MAX_LENGTH 100

#define CORE_IP "127.0.0.1"
#define CORE_PORT 8080

char instructions[MAX_INSTRUCTIONS][MAX_LENGTH];
int instruction_count = 0;

/* Connect to Core and send a command using TCP sockets */
int send_command_to_core(const char *command)
{
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        perror("Socket creation failed");
        return 0;
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CORE_PORT);

    if (inet_pton(AF_INET, CORE_IP,
                  &server_addr.sin_addr) != 1) {
        printf("Invalid Core IP address.\n");
        close(sockfd);
        return 0;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {
        perror("Unable to connect to Core");
        close(sockfd);
        return 0;
    }

    size_t length = strlen(command);
    size_t sent = 0;

    while (sent < length) {
        ssize_t result = send(sockfd, command + sent,
                              length - sent, 0);

        if (result <= 0) {
            perror("Failed to send command");
            close(sockfd);
            return 0;
        }

        sent += (size_t)result;
    }

    printf("Command sent to Core through socket: %s\n",
           command);

    close(sockfd);
    return 1;
}

void show_menu(void)
{
    printf("\n========================================\n");
    printf("          CPU SIMULATOR - UI\n");
    printf("========================================\n");
    printf("1. Load Program\n");
    printf("2. Run Program\n");
    printf("3. Execute One Instruction\n");
    printf("4. View CPU Registers\n");
    printf("5. View Memory\n");
    printf("6. View Execution Status\n");
    printf("7. Help\n");
    printf("0. Exit\n");
    printf("----------------------------------------\n");
    printf("Enter your choice: ");
}

void load_program(void)
{
    char filename[256];

    printf("Enter program filename: ");

    if (scanf("%255s", filename) != 1) {
        printf("Invalid filename.\n");
        return;
    }

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Could not open file");
        return;
    }

    instruction_count = 0;

    while (instruction_count < MAX_INSTRUCTIONS &&
           fgets(instructions[instruction_count],
                 MAX_LENGTH, file) != NULL) {
        instruction_count++;
    }

    fclose(file);

    printf("Successfully loaded %d instruction lines.\n",
           instruction_count);
}

void show_help(void)
{
    printf("\n========== CPU SIMULATOR HELP ==========\n");
    printf("1. Load Program: Read instructions from a file.\n");
    printf("2. Run Program: Request full program execution.\n");
    printf("3. Execute One Instruction: Request one step.\n");
    printf("4. View CPU Registers: Display register values.\n");
    printf("5. View Memory: Display memory contents.\n");
    printf("6. Execution Status: Display CPU status.\n");
    printf("7. Help: Display this help message.\n");
    printf("0. Exit: Close the simulator UI.\n");
    printf("=======================================\n");
}

void show_registers(void)
{
    printf("\n========== CPU REGISTERS ==========\n");
    printf("R0 = 0\n");
    printf("R1 = 0\n");
    printf("R2 = 0\n");
    printf("R3 = 0\n");
    printf("Program Counter (PC) = 0\n");
    printf("===================================\n");
}

void show_memory(void)
{
    printf("\n========== CPU MEMORY ==========\n");
    printf("Address 0: 10\n");
    printf("Address 1: 20\n");
    printf("Address 2: 0\n");
    printf("Address 3: 0\n");
    printf("================================\n");
}

void show_execution_status(void)
{
    printf("\n======= EXECUTION STATUS =======\n");
    printf("Program loaded: %s\n",
           instruction_count > 0 ? "Yes" : "No");
    printf("Instructions loaded: %d\n", instruction_count);
    printf("Execution state: Not started\n");
    printf("================================\n");
}

int main(void)
{
    int choice;

    while (1) {
        show_menu();

        if (scanf("%d", &choice) != 1) {
            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Clear invalid input */
            }

            printf("Invalid input. Enter a menu number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                load_program();
                break;

            case 2:
                if (send_command_to_core("RUN\n")) {
                    printf("RUN command sent successfully.\n");
                } else {
                    printf("Failed to send RUN command.\n");
                }
                break;

            case 3:
                printf("Single-step execution selected.\n");
                break;

            case 4:
                show_registers();
                break;

            case 5:
                show_memory();
                break;

            case 6:
                show_execution_status();
                break;

            case 7:
                show_help();
                break;

            case 0:
                printf("Exiting CPU Simulator UI.\n");
                return EXIT_SUCCESS;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
