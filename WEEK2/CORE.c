#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ipc.h"

#define MAX_MEMORY 1024
#define MAX_STACK 100
#define MAX_QUEUE 100

// ==================== CPU MODULE ====================

int cpu_running = 0;
int cpu_loaded = 0;
int instructions_executed = 0;

void load_cpu(void)
{
    if (cpu_loaded) {
        printf("\nProgram is already loaded.\n");
        return;
    }

    cpu_loaded = 1;
    instructions_executed = 0;

    printf("\nProgram loaded into CPU successfully.\n");
}

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

    for (int i = 1; i <= 5; i++) {
        instructions_executed++;
        printf("Executing instruction %d\n",
               instructions_executed);
    }

    cpu_running = 0;

    printf("CPU execution completed.\n");
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

    printf("\nCPU reset successfully.\n");
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

/* ==================== MESSAGE QUEUE WORKER ==================== */

void process_core_message(const IPCMessage *message)
{
    if (message == NULL)
        return;

    printf("\nMessage Queue received command: %d\n", message->command);

    switch (message->command)
    {
        case IPC_CMD_LOAD:
            load_cpu();
            break;

        case IPC_CMD_RUN:
            load_cpu();
            execute_cpu();
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

            if (strncmp(buffer, "RUN", 3) == 0)
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

    start_core_socket_server();

    printf("\nCore Process stopped.\n");

    return 0;
}