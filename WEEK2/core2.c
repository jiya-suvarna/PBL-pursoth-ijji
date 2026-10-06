#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX 5
#define FIFO_NAME "/tmp/simulator_log_fifo"

/* =========================
   QUEUE VARIABLES
   ========================= */

int queue[MAX];
int front = -1;
int rear = -1;

/* =========================
   STACK VARIABLES
   ========================= */

int stack[MAX];
int top = -1;

/* =========================
   LOGGER FUNCTION
   ========================= */

void send_log(int fd, const char *message)
{
    if (write(fd, message, strlen(message) + 1) == -1)
    {
        perror("Error writing to Logger");
    }
}

/* =========================================================
   QUEUE FUNCTIONS
   ========================================================= */

void enqueue(int fd)
{
    int value;
    char message[1024];

    if (rear == MAX - 1)
    {
        printf("Queue Overflow\n");

        strcpy(message, "WARNING: Queue Overflow");
        send_log(fd, message);
    }
    else
    {
        printf("Enter value: ");
        scanf("%d", &value);

        if (front == -1)
        {
            front = 0;
        }

        rear++;
        queue[rear] = value;

        printf("%d added to queue\n", value);

        snprintf(message, sizeof(message),
                 "INFO: Value %d added to queue", value);

        send_log(fd, message);
    }
}

void dequeue(int fd)
{
    char message[1024];

    if (front == -1 || front > rear)
    {
        printf("Queue Underflow\n");

        strcpy(message, "WARNING: Queue Underflow");
        send_log(fd, message);
    }
    else
    {
        int value = queue[front];

        printf("%d removed from queue\n", value);

        front++;

        snprintf(message, sizeof(message),
                 "INFO: Value %d removed from queue", value);

        send_log(fd, message);

        if (front > rear)
        {
            front = -1;
            rear = -1;
        }
    }
}

void display_queue(int fd)
{
    int i;
    char message[1024];

    if (front == -1)
    {
        printf("Queue is empty\n");

        strcpy(message, "INFO: Queue is empty");
        send_log(fd, message);
    }
    else
    {
        printf("Queue: ");

        for (i = front; i <= rear; i++)
        {
            printf("%d ", queue[i]);
        }

        printf("\n");

        strcpy(message, "INFO: Queue displayed successfully");
        send_log(fd, message);
    }
}

/* =========================================================
   STACK FUNCTIONS
   ========================================================= */

void push(int fd)
{
    int value;
    char message[1024];

    if (top == MAX - 1)
    {
        printf("Stack Overflow\n");

        strcpy(message, "WARNING: Stack Overflow");
        send_log(fd, message);
    }
    else
    {
        printf("Enter value: ");
        scanf("%d", &value);

        top++;
        stack[top] = value;

        printf("%d pushed into stack\n", value);

        snprintf(message, sizeof(message),
                 "INFO: Value %d pushed into stack", value);

        send_log(fd, message);
    }
}

void pop(int fd)
{
    char message[1024];

    if (top == -1)
    {
        printf("Stack Underflow\n");

        strcpy(message, "WARNING: Stack Underflow");
        send_log(fd, message);
    }
    else
    {
        int value = stack[top];

        printf("%d popped from stack\n", value);

        top--;

        snprintf(message, sizeof(message),
                 "INFO: Value %d popped from stack", value);

        send_log(fd, message);
    }
}

void display_stack(int fd)
{
    int i;
    char message[1024];

    if (top == -1)
    {
        printf("Stack is empty\n");

        strcpy(message, "INFO: Stack is empty");
        send_log(fd, message);
    }
    else
    {
        printf("Stack: ");

        for (i = top; i >= 0; i--)
        {
            printf("%d ", stack[i]);
        }

        printf("\n");

        strcpy(message, "INFO: Stack displayed successfully");
        send_log(fd, message);
    }
}

/* =========================================================
   MAIN CORE PROCESS
   ========================================================= */

int main()
{
    int fd;
    int choice;
    char message[1024];

    printf("\n====================================\n");
    printf("          CORE PROCESS\n");
    printf("====================================\n");

    printf("Connecting to Logger...\n");

    /* Open FIFO for writing */
    fd = open(FIFO_NAME, O_WRONLY);

    if (fd == -1)
    {
        perror("Error opening FIFO");
        return 1;
    }

    printf("Connected to Logger!\n");

    /* Send Core Process start message */
    strcpy(message, "INFO: Core process started successfully");
    send_log(fd, message);

    sleep(1);

    /* Inform Logger about modules */
    strcpy(message, "INFO: Queue and Stack modules initialized");
    send_log(fd, message);

    printf("\nQueue and Stack modules initialized successfully.\n");

    /* =====================================================
       MAIN MENU
       ===================================================== */

    while (1)
    {
        printf("\n====================================\n");
        printf("           CORE PROCESS MENU\n");
        printf("====================================\n");

        printf("1. Queue Operations\n");
        printf("2. Stack Operations\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        /* ================================================
           QUEUE MENU
           ================================================ */

        if (choice == 1)
        {
            int queue_choice;

            while (1)
            {
                printf("\n----------- QUEUE MENU -----------\n");
                printf("1. Enqueue\n");
                printf("2. Dequeue\n");
                printf("3. Display\n");
                printf("4. Back to Main Menu\n");

                printf("Enter choice: ");
                scanf("%d", &queue_choice);

                switch (queue_choice)
                {
                    case 1:
                        enqueue(fd);
                        break;

                    case 2:
                        dequeue(fd);
                        break;

                    case 3:
                        display_queue(fd);
                        break;

                    case 4:
                        printf("Returning to Main Menu...\n");
                        break;

                    default:
                        printf("Invalid Queue choice\n");

                        strcpy(message,
                               "WARNING: Invalid Queue menu choice");

                        send_log(fd, message);
                }

                if (queue_choice == 4)
                {
                    break;
                }
            }
        }

        /* ================================================
           STACK MENU
           ================================================ */

        else if (choice == 2)
        {
            int stack_choice;

            while (1)
            {
                printf("\n----------- STACK MENU -----------\n");
                printf("1. Push\n");
                printf("2. Pop\n");
                printf("3. Display\n");
                printf("4. Back to Main Menu\n");

                printf("Enter choice: ");
                scanf("%d", &stack_choice);

                switch (stack_choice)
                {
                    case 1:
                        push(fd);
                        break;

                    case 2:
                        pop(fd);
                        break;

                    case 3:
                        display_stack(fd);
                        break;

                    case 4:
                        printf("Returning to Main Menu...\n");
                        break;

                    default:
                        printf("Invalid Stack choice\n");

                        strcpy(message,
                               "WARNING: Invalid Stack menu choice");

                        send_log(fd, message);
                }

                if (stack_choice == 4)
                {
                    break;
                }
            }
        }

        /* ================================================
           EXIT
           ================================================ */

        else if (choice == 3)
        {
            strcpy(message, "INFO: Core process completed");
            send_log(fd, message);

            close(fd);

            printf("\n====================================\n");
            printf("     CORE PROCESS COMPLETED\n");
            printf("====================================\n");

            printf("All messages sent to Logger.\n");

            return 0;
        }

        else
        {
            printf("Invalid choice\n");

            strcpy(message, "WARNING: Invalid Core menu choice");
            send_log(fd, message);
        }
    }

    return 0;
}