#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ipc.h"

#define FIFO_NAME "/tmp/simulator_log_fifo"

#define MAX_MEMORY 1024
#define MAX_STACK 100
#define MAX_QUEUE 100

int logger_fd = -1;

// ==================== CPU MODULE ====================

int cpu_running = 0;
int cpu_loaded = 0;
int instructions_executed = 0;

#define MAX_INSTRUCTIONS 100

char program_instructions[MAX_INSTRUCTIONS][128];
int program_count = 0;

int registers[4] = {0};
int program_counter = 0;

int simulated_memory[MAX_MEMORY] = {0};

void load_cpu(void)
{
    if (program_count == 0) {
        printf("\nNo instructions received from UI.\n");
        return;
    }

    cpu_loaded = 1;
    cpu_running = 0;
    instructions_executed = 0;
    program_counter = 0;

    for (int i = 0; i < 4; i++) {
        registers[i] = 0;
    }

    printf("\nProgram loaded into CPU successfully.\n");
    printf("Total instructions: %d\n", program_count);
}

void execute_one_instruction(void);

void execute_cpu(void)
{
    if (!cpu_loaded) {
        printf("\nPlease load a program first.\n");
        return;
    }

    if (cpu_running) {
        printf("\nCPU is already running.\n");
        return;
    }

    cpu_running = 1;
    printf("\nCPU execution started.\n");

    while (program_counter < program_count) {
        execute_one_instruction();
    }

    cpu_running = 0;
    printf("\nCPU execution completed.\n");
}

void execute_one_instruction(void)
{
    if (!cpu_loaded) {
        printf("\nPlease load a program first.\n");
        return;
    }

    if (program_counter >= program_count) {
        printf("\nAll instructions have been executed.\n");
        return;
    }

    char *instruction = program_instructions[program_counter];
    int reg1, reg2, value, address;

    printf("\nExecuting: %s\n", instruction);

    if (sscanf(instruction, "LOAD R%d, %d", &reg1, &value) == 2) {
        if (reg1 >= 0 && reg1 < 4) {
            registers[reg1] = value;
            printf("R%d = %d\n", reg1, value);
        }
    }
    else if (sscanf(instruction, "ADD R%d, R%d", &reg1, &reg2) == 2) {
        if (reg1 >= 0 && reg1 < 4 &&
            reg2 >= 0 && reg2 < 4) {
            registers[reg1] += registers[reg2];
            printf("R%d = %d\n", reg1, registers[reg1]);
        }
    }
    else if (sscanf(instruction, "STORE R%d, %d", &reg1, &address) == 2) {
        if (reg1 >= 0 && reg1 < 4 &&
            address >= 0 && address < MAX_MEMORY) {
            simulated_memory[address] = registers[reg1];
            printf("Memory[%d] = %d\n",
                   address, simulated_memory[address]);
        }
    }
    else {
        printf("Invalid instruction: %s\n", instruction);
    }

    program_counter++;
    instructions_executed++;
}

void stop_cpu(void)
{
    if (!cpu_running) {
        printf("\nCPU is not running.\n");
        return;
    }

    cpu_running = 0;
    printf("\nCPU stopped.\n");
}

void reset_cpu(void)
{
    cpu_running = 0;
    cpu_loaded = 0;
    instructions_executed = 0;
    program_counter = 0;
    program_count = 0;

    for (int i = 0; i < 4; i++) {
        registers[i] = 0;
    }

    for (int i = 0; i < MAX_MEMORY; i++) {
        simulated_memory[i] = 0;
    }

    for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
        program_instructions[i][0] = '\0';
    }

    printf("\nCPU reset successfully.\n");
    printf("Registers, memory and program cleared.\n");
}

void cpu_status(void)
{
    printf("\n========== CPU STATUS ==========\n");
    printf("Program loaded: %s\n",
           cpu_loaded ? "Yes" : "No");
    printf("CPU running: %s\n",
           cpu_running ? "Yes" : "No");
    printf("Instructions executed: %d\n",
           instructions_executed);
    printf("================================\n");
}

// ==================== MEMORY MODULE ====================

int memory_used = 0;

void allocate_memory(void)
{
    int size;

    printf("\nEnter memory size to allocate: ");

    if (scanf("%d", &size) != 1) {
        printf("Invalid input.\n");
        return;
    }

    if (size <= 0) {
        printf("Memory size must be positive.\n");
        return;
    }

    if (size > MAX_MEMORY - memory_used) {
        printf("Insufficient memory available.\n");
        return;
    }

    memory_used += size;

    printf("%d units of memory allocated successfully.\n",
           size);
}

void release_memory(void)
{
    int size;

    printf("\nEnter memory size to release: ");

    if (scanf("%d", &size) != 1) {
        printf("Invalid input.\n");
        return;
    }

    if (size <= 0) {
        printf("Memory size must be positive.\n");
        return;
    }

    if (size > memory_used) {
        printf("Cannot release more memory than is allocated.\n");
        return;
    }

    memory_used -= size;

    printf("%d units of memory released successfully.\n",
           size);
}

void memory_status(void)
{
    printf("\n========== MEMORY STATUS ==========\n");
    printf("Total memory: %d units\n", MAX_MEMORY);
    printf("Used memory: %d units\n", memory_used);
    printf("Free memory: %d units\n",
           MAX_MEMORY - memory_used);
    printf("===================================\n");
}

// ==================== STACK MODULE ====================

int stack[MAX_STACK];
int top = -1;

void push_stack(void)
{
    int value;

    if (top == MAX_STACK - 1) {
        printf("\nStack overflow.\n");
        return;
    }

    printf("\nEnter value to push: ");

    if (scanf("%d", &value) != 1) {
        printf("Invalid input.\n");
        return;
    }

    stack[++top] = value;

    printf("%d pushed onto stack.\n", value);
}

void pop_stack(void)
{
    if (top == -1) {
        printf("\nStack underflow. Stack is empty.\n");
        return;
    }

    printf("\nPopped element: %d\n", stack[top--]);
}

void display_stack(void)
{
    if (top == -1) {
        printf("\nStack is empty.\n");
        return;
    }

    printf("\nStack (top to bottom):\n");

    for (int i = top; i >= 0; i--) {
        printf("%d\n", stack[i]);
    }
}

// ==================== QUEUE MODULE ====================

int queue[MAX_QUEUE];
int front = -1;
int rear = -1;

void enqueue(void)
{
    int value;

    if (rear == MAX_QUEUE - 1) {
        printf("\nQueue is full.\n");
        return;
    }

    printf("\nEnter value to insert: ");

    if (scanf("%d", &value) != 1) {
        printf("Invalid input.\n");
        return;
    }

    if (front == -1) {
        front = 0;
    }

    queue[++rear] = value;

    printf("%d inserted into queue.\n", value);
}

void dequeue(void)
{
    if (front == -1) {
        printf("\nQueue is empty.\n");
        return;
    }

    printf("\nRemoved element: %d\n", queue[front]);

    if (front == rear) {
        front = -1;
        rear = -1;
    } else {
        front++;
    }
}

void display_queue(void)
{
    if (front == -1) {
        printf("\nQueue is empty.\n");
        return;
    }

    printf("\nQueue (front to rear): ");

    for (int i = front; i <= rear; i++) {
        printf("%d ", queue[i]);
    }

    printf("\n");
}

// ==================== CPU MENU ====================

void cpu_menu(void)
{
    int choice;

    do {
        printf("\n========== CPU MENU ==========\n");
        printf("1. Load Program\n");
        printf("2. Execute Program\n");
        printf("3. Stop CPU\n");
        printf("4. Reset CPU\n");
        printf("5. CPU Status\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return;
        }

        switch (choice) {
            case 1:
                load_cpu();
                break;

            case 2:
                execute_cpu();
                break;

            case 3:
                stop_cpu();
                break;

            case 4:
                reset_cpu();
                break;

            case 5:
                cpu_status();
                break;

            case 0:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);
}

// ==================== MEMORY MENU ====================

void memory_menu(void)
{
    int choice;

    do {
        printf("\n========== MEMORY MENU ==========\n");
        printf("1. Allocate Memory\n");
        printf("2. Release Memory\n");
        printf("3. Memory Status\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return;
        }

        switch (choice) {
            case 1:
                allocate_memory();
                break;

            case 2:
                release_memory();
                break;

            case 3:
                memory_status();
                break;

            case 0:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);
}

// ==================== STACK MENU ====================

void stack_menu(void)
{
    int choice;

    do {
        printf("\n========== STACK MENU ==========\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Display Stack\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return;
        }

        switch (choice) {
            case 1:
                push_stack();
                break;

            case 2:
                pop_stack();
                break;

            case 3:
                display_stack();
                break;

            case 0:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);
}

// ==================== QUEUE MENU ====================

void queue_menu(void)
{
    int choice;

    do {
        printf("\n========== QUEUE MENU ==========\n");
        printf("1. Enqueue\n");
        printf("2. Dequeue\n");
        printf("3. Display Queue\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return;
        }

        switch (choice) {
            case 1:
                enqueue();
                break;

            case 2:
                dequeue();
                break;

            case 3:
                display_queue();
                break;

            case 0:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);
}

/* ==================== LOGGER FIFO COMMUNICATION ==================== */

void send_log_to_logger(const char *message)
{
    if (logger_fd == -1 || message == NULL)
        return;

    char buffer[256];

    snprintf(buffer, sizeof(buffer), "INFO:%s", message);

    write(logger_fd, buffer, strlen(buffer));
}

void connect_to_logger(void)
{
    logger_fd = open(FIFO_NAME, O_WRONLY);

    if (logger_fd == -1)
    {
        perror("Failed to connect to Logger");
        return;
    }

    printf("Core connected to Logger through FIFO.\n");
}

/* ==================== MESSAGE QUEUE WORKER ==================== */

void process_core_message(const IPCMessage *message)
{
    if (message == NULL)
        return;

    printf("\nMessage Queue received command: %d\n", message->command);

    switch (message->command)
    {

                case IPC_CMD_INSTRUCTION:
            if (program_count < MAX_INSTRUCTIONS) {
                snprintf(program_instructions[program_count],
                         sizeof(program_instructions[program_count]),
                         "%s", message->text);
                program_count++;
                printf("Instruction received: %s\n",
                       message->text);
            }
            break;

        case IPC_CMD_ENDLOAD:
    printf("All %d instructions received from UI.\n",
           program_count);
    load_cpu();
    break;

    case IPC_CMD_VIEW_REGISTERS:
    printf("\n========== CPU REGISTERS ==========\n");
    for (int i = 0; i < 4; i++) {
        printf("R%d = %d\n", i, registers[i]);
    }
    printf("Program Counter (PC) = %d\n", program_counter);
    printf("===================================\n");
    break;

    case IPC_CMD_VIEW_MEMORY:
    printf("\n========== MEMORY CONTENTS ==========\n");

    for (int i = 0; i < 10; i++) {
        printf("Memory[%d] = %d\n", i, simulated_memory[i]);
    }

    printf("=====================================\n");
    break;

    case IPC_CMD_VIEW_STATUS:
    printf("\n========== EXECUTION STATUS ==========\n");
    printf("CPU Loaded: %s\n", cpu_loaded ? "Yes" : "No");
    printf("CPU Running: %s\n", cpu_running ? "Yes" : "No");
    printf("Instructions Executed: %d\n", instructions_executed);
    printf("Total Instructions: %d\n", program_count);
    printf("Program Counter (PC): %d\n", program_counter);
    printf("======================================\n");
    break;
         
        case IPC_CMD_LOAD:
    program_count = 0;
    cpu_loaded = 0;
    program_counter = 0;
    instructions_executed = 0;
    printf("\nReceiving new program from UI...\n");
    break;

        case IPC_CMD_RUN:
    if (cpu_loaded) {
        execute_cpu();
    } else {
        printf("Please load a program first.\n");
    }
    break; 

        case IPC_CMD_STEP:
            execute_one_instruction();
            break;

        case IPC_CMD_STOP:
            stop_cpu();
            break;

        case IPC_CMD_RESET:
            reset_cpu();
            break;

        case IPC_CMD_EXIT:
            printf("Exit command received through Message Queue.\n");
            break;

        default:
            printf("Unknown Message Queue command.\n");
            break;
    }

        send_log_to_logger("Core processed a command");
}

/* ==================== CORE SOCKET SERVER ==================== */

void start_core_socket_server(void)
{
    int server_fd;
    int client_fd;
    mqd_t core_queue;
    struct sockaddr_in server_addr;
    char buffer[128];

    core_queue = ipc_open_queue(UI_TO_CORE_QUEUE, O_RDWR, 1);

if (core_queue == (mqd_t)-1)
{
    perror("Failed to open Core Message Queue");
    return;
}

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        perror("Core socket creation failed");
        return;
    }

    int option = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               &option, sizeof(option));

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_fd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == -1)
    {
        perror("Core socket bind failed");
        close(server_fd);
        return;
    }

    if (listen(server_fd, 5) == -1)
    {
        perror("Core socket listen failed");
        close(server_fd);
        return;
    }

    printf("\nCore socket server started on port 8080.\n");
    printf("Waiting for commands from UI...\n");

    while (1)
    {
        client_fd = accept(server_fd, NULL, NULL);

        if (client_fd == -1)
        {
            perror("Core accept failed");
            continue;
        }

        memset(buffer, 0, sizeof(buffer));

        ssize_t bytes_received =
            recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';

            printf("\nCommand received from UI: %s", buffer);

                        IPCMessage message;
            IPCMessage received_message;

            memset(&message, 0, sizeof(message));

            message.type = IPC_MSG_COMMAND;

if (strncmp(buffer, "LOAD", 4) == 0)
{
    message.command = IPC_CMD_LOAD;
}
else if (strncmp(buffer, "RUN", 3) == 0)
{
    message.command = IPC_CMD_RUN;
}
else if (strncmp(buffer, "STOP", 4) == 0)
{
    message.command = IPC_CMD_STOP;
}
else if (strncmp(buffer, "RESET", 5) == 0)
{
    message.command = IPC_CMD_RESET;
}
else if (strncmp(buffer, "STEP", 4) == 0) 
{
    message.command = IPC_CMD_STEP;
}
else if (strncmp(buffer, "INSTRUCTION:", 12) == 0)
{
    message.command = IPC_CMD_INSTRUCTION;
    snprintf(message.text, sizeof(message.text),
             "%s", buffer + 12);
}
else if (strncmp(buffer, "ENDLOAD", 7) == 0)
{
    message.command = IPC_CMD_ENDLOAD;
}
else if (strncmp(buffer, "VIEW_REGISTERS", 14) == 0)
{
    message.command = IPC_CMD_VIEW_REGISTERS;
}
else if (strncmp(buffer, "VIEW_MEMORY", 11) == 0)
{
    message.command = IPC_CMD_VIEW_MEMORY;
}
else if (strncmp(buffer, "VIEW_STATUS", 11) == 0)
{
    message.command = IPC_CMD_VIEW_STATUS;
}
else if (strncmp(buffer, "EXIT", 4) == 0)
{
    message.command = IPC_CMD_EXIT;
}
else
{
    printf("Unknown command received from UI.\n");
    message.command = 0;
}

            if (message.command != 0)
            {
                if (ipc_send(core_queue, &message))
                {
                    printf("Command sent through POSIX Message Queue.\n");

                    if (ipc_receive(core_queue, &received_message))
                    {
                        process_core_message(&received_message);

                      if (received_message.command == IPC_CMD_VIEW_REGISTERS) {
    char response[512];
    int length = snprintf(response, sizeof(response),
        "R0 = %d\nR1 = %d\nR2 = %d\nR3 = %d\nProgram Counter (PC) = %d\n",
        registers[0], registers[1], registers[2],
        registers[3], program_counter);

    send(client_fd, response, length, 0);
}  
if (received_message.command == IPC_CMD_VIEW_MEMORY) {
    char response[512];
    int length = 0;

    for (int i = 0; i < 10; i++) {
        length += snprintf(response + length,
                           sizeof(response) - length,
                           "Memory[%d] = %d\n",
                           i, simulated_memory[i]);
    }

    send(client_fd, response, length, 0);
}
if (received_message.command == IPC_CMD_VIEW_STATUS) {
    char response[512];

    int length = snprintf(response, sizeof(response),
        "CPU Loaded: %s\n"
        "CPU Running: %s\n"
        "Instructions Executed: %d\n"
        "Total Instructions: %d\n"
        "Program Counter (PC): %d\n",
        cpu_loaded ? "Yes" : "No",
        cpu_running ? "Yes" : "No",
        instructions_executed,
        program_count,
        program_counter);

    send(client_fd, response, length, 0);
}
                    }
                    else
                    {
                        perror("Message Queue receive failed");
                    }
                }
                else
                {
                    perror("Message Queue send failed");
                }
            }

            if (message.command == IPC_CMD_EXIT)
            {
                close(client_fd);
                break;
            }
        }

        close(client_fd);
    }

    mq_close(core_queue);

    close(server_fd);

    printf("Core socket server stopped.\n");
}

// ==================== MAIN MENU ====================

int main(void)
{
    printf("\n====================================\n");
    printf("       CORE PROCESS STARTED\n");
    printf("====================================\n");

    connect_to_logger();

    start_core_socket_server();

    printf("\nCore Process stopped.\n");

    return 0;
}