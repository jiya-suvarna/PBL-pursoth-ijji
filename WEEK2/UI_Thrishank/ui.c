#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include "ipc.h"

#define MAX_INSTRUCTIONS 100
#define MAX_LENGTH 100

char instructions[MAX_INSTRUCTIONS][MAX_LENGTH];
int instruction_count = 0;


/* ================= POSIX MESSAGE QUEUE COMMUNICATION ================= */

static mqd_t ui_to_core_queue = (mqd_t)-1;
static mqd_t core_to_ui_queue = (mqd_t)-1;

char core_response[IPC_TEXT_SIZE];

int connect_to_core(void)
{
    ui_to_core_queue = ipc_open_queue(
        UI_TO_CORE_QUEUE, O_WRONLY, 0
    );

    if (ui_to_core_queue == (mqd_t)-1)
    {
        perror("Unable to open UI-to-Core queue");
        return 0;
    }

    core_to_ui_queue = ipc_open_queue(
        CORE_TO_UI_QUEUE, O_RDONLY, 0
    );

    if (core_to_ui_queue == (mqd_t)-1)
    {
        perror("Unable to open Core-to-UI queue");
        mq_close(ui_to_core_queue);
        ui_to_core_queue = (mqd_t)-1;
        return 0;
    }

    printf("UI connected to Core using POSIX Message Queues.\n");
    return 1;
}

int send_command_to_core(const char *command)
{
    IPCMessage message;
    IPCMessage response;

    memset(&message, 0, sizeof(message));
    memset(&response, 0, sizeof(response));

    if (strncmp(command, "LOAD", 4) == 0)
        message.command = IPC_CMD_LOAD;
    else if (strncmp(command, "RUN", 3) == 0)
        message.command = IPC_CMD_RUN;
    else if (strncmp(command, "STEP", 4) == 0)
        message.command = IPC_CMD_STEP;
    else if (strncmp(command, "STOP", 4) == 0)
        message.command = IPC_CMD_STOP;
    else if (strncmp(command, "RESET", 5) == 0)
        message.command = IPC_CMD_RESET;
    else if (strncmp(command, "EXIT", 4) == 0)
        message.command = IPC_CMD_EXIT;
    else if (strncmp(command, "VIEW_REGISTERS", 14) == 0)
        message.command = IPC_CMD_VIEW_REGISTERS;
    else if (strncmp(command, "VIEW_MEMORY", 11) == 0)
        message.command = IPC_CMD_VIEW_MEMORY;
    else if (strncmp(command, "VIEW_STATUS", 11) == 0)
        message.command = IPC_CMD_VIEW_STATUS;
    else if (strncmp(command, "INSTRUCTION:", 12) == 0)
    {
        message.command = IPC_CMD_INSTRUCTION;
        snprintf(message.text, sizeof(message.text),
                 "%.511s", command + 12);
    }
        else if (strncmp(command, "ENDLOAD", 7) == 0)
        message.command = IPC_CMD_ENDLOAD;

    else if (strncmp(command, "ALLOCATE_MEMORY:", 16) == 0)
    {
        message.command = IPC_CMD_ALLOCATE_MEMORY;
        snprintf(message.text, sizeof(message.text), "%.511s", command + 16);
    }
    else if (strncmp(command, "RELEASE_MEMORY:", 15) == 0)
    {
        message.command = IPC_CMD_RELEASE_MEMORY;
        snprintf(message.text, sizeof(message.text), "%.511s", command + 15);
    }
    else if (strcmp(command, "MEMORY_STATUS") == 0)
        message.command = IPC_CMD_MEMORY_STATUS;

    else if (strncmp(command, "STACK_PUSH:", 11) == 0)
    {
        message.command = IPC_CMD_STACK_PUSH;
        snprintf(message.text, sizeof(message.text), "%.511s", command + 11);
    }
    else if (strcmp(command, "STACK_POP") == 0)
        message.command = IPC_CMD_STACK_POP;
    else if (strcmp(command, "STACK_DISPLAY") == 0)
        message.command = IPC_CMD_STACK_DISPLAY;

    else if (strncmp(command, "QUEUE_ENQUEUE:", 14) == 0)
    {
        message.command = IPC_CMD_QUEUE_ENQUEUE;
        snprintf(message.text, sizeof(message.text), "%.511s", command + 14);
    }
    else if (strcmp(command, "QUEUE_DEQUEUE") == 0)
        message.command = IPC_CMD_QUEUE_DEQUEUE;
    else if (strcmp(command, "QUEUE_DISPLAY") == 0)
        message.command = IPC_CMD_QUEUE_DISPLAY;

    else
    {
        printf("Unknown command.\n");
        return 0;
    }

    message.type = IPC_MSG_COMMAND;

    if (!ipc_send(ui_to_core_queue, &message))
    {
        perror("Failed to send message to Core");
        return 0;
    }

    if (!ipc_receive(core_to_ui_queue, &response))
    {
        perror("Failed to receive response from Core");
        return 0;
    }

    if (response.type != IPC_MSG_RESPONSE)
    {
        printf("Invalid response received from Core.\n");
        return 0;
    }

    snprintf(core_response, sizeof(core_response),
             "%s", response.text);

    return response.status;
}

void request_registers_from_core(void)
{
    if (send_command_to_core("VIEW_REGISTERS"))
        printf("\n========== CPU REGISTERS ==========\n%s",
               core_response);
}

void request_memory_from_core(void)
{
    if (send_command_to_core("VIEW_MEMORY"))
        printf("\n========== MEMORY CONTENTS ==========\n%s",
               core_response);
}

void request_status_from_core(void)
{
    if (send_command_to_core("VIEW_STATUS"))
        printf("\n========== EXECUTION STATUS ==========\n%s",
               core_response);
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
    printf("8. Reset CPU\n");
    printf("9. Memory / Stack / Queue\n");
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
    printf("8. Reset CPU: Clear registers, memory and program.\n");
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

void memory_stack_queue_menu(void)
{
    int choice, value;
    char command[100];

    do {
        printf("\n===== MEMORY / STACK / QUEUE =====\n");
        printf("1. Allocate Memory\n");
        printf("2. Release Memory\n");
        printf("3. Memory Status\n");
        printf("4. Push\n");
        printf("5. Pop\n");
        printf("6. Display Stack\n");
        printf("7. Enqueue\n");
        printf("8. Dequeue\n");
        printf("9. Display Queue\n");
        printf("0. Back\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
            return;

        command[0] = '\0';

        switch (choice) {
            case 1:
            case 2:
            case 4:
            case 7:
                printf("Enter value: ");
                if (scanf("%d", &value) != 1)
                    return;

                if (choice == 1)
                    snprintf(command, sizeof(command), "ALLOCATE_MEMORY:%d", value);
                else if (choice == 2)
                    snprintf(command, sizeof(command), "RELEASE_MEMORY:%d", value);
                else if (choice == 4)
                    snprintf(command, sizeof(command), "STACK_PUSH:%d", value);
                else
                    snprintf(command, sizeof(command), "QUEUE_ENQUEUE:%d", value);
                break;

            case 3: strcpy(command, "MEMORY_STATUS"); break;
            case 5: strcpy(command, "STACK_POP"); break;
            case 6: strcpy(command, "STACK_DISPLAY"); break;
            case 8: strcpy(command, "QUEUE_DEQUEUE"); break;
            case 9: strcpy(command, "QUEUE_DISPLAY"); break;
            case 0: break;
            default:
                printf("Invalid choice.\n");
        }

        if (command[0] != '\0') {
            if (send_command_to_core(command))
                printf("\n%s\n", core_response);
        }

    } while (choice != 0);
}

int main(void)
{
    if (!connect_to_core())
{
    fprintf(stderr, "Unable to connect to Core. Start Core first.\n");
    return EXIT_FAILURE;
}
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

    if (instruction_count == 0) {
        printf("No instructions to send.\n");
        break;
    }

    if (!send_command_to_core("LOAD\n")) {
        printf("Failed to send LOAD command.\n");
        break;
    }

    for (int i = 0; i < instruction_count; i++) {
        char message[MAX_LENGTH + 20];

        snprintf(message, sizeof(message),
                 "INSTRUCTION:%s", instructions[i]);

        if (!send_command_to_core(message)) {
            printf("Failed to send instruction %d.\n", i + 1);
            break;
        }
    }

    send_command_to_core("ENDLOAD\n");
    printf("Program sent to Core.\n");
    break;

            case 2:
                if (send_command_to_core("RUN\n")) {
                    printf("RUN command sent successfully.\n");
                } else {
                    printf("Failed to send RUN command.\n");
                }
                break;

            case 3:
    if (send_command_to_core("STEP\n")) {
        printf("Single-step command sent successfully.\n");
    } else {
      printf("Failed to send single-step command.\n");
    }
    break;

            case 4:
                request_registers_from_core();
                break;

            case 5:
                request_memory_from_core();
                break;

            case 6:
                request_status_from_core();
                break;

            case 7:
                show_help();
                break;

            case 8:
    if (send_command_to_core("RESET\n")) {
        printf("CPU reset command sent successfully.\n");
    } else {
        printf("Failed to send reset command.\n");
    }
    break;

                      case 9:
                memory_stack_queue_menu();
                break;

           case 0:
    printf("Shutting down CPU Simulator...\n");

    send_command_to_core("EXIT");

    if (ui_to_core_queue != (mqd_t)-1)
        mq_close(ui_to_core_queue);

    if (core_to_ui_queue != (mqd_t)-1)
        mq_close(core_to_ui_queue);

    printf("UI process stopped successfully.\n");
    return EXIT_SUCCESS;

        }
    }
}
