#include <stdio.h>

#define MAX_SIZE 100

/* =========================
   STACK
   ========================= */

int stack[MAX_SIZE];
int top = -1;

void push(int value)
{
    if (top == MAX_SIZE - 1)
    {
        printf("Stack Overflow\n");
        return;
    }

    top++;
    stack[top] = value;

    printf("Pushed %d into Stack\n", value);
}

void pop(void)
{
    if (top == -1)
    {
        printf("Stack Underflow\n");
        return;
    }

    printf("Popped %d from Stack\n", stack[top]);
    top--;
}

void display_stack(void)
{
    int i;

    if (top == -1)
    {
        printf("Stack is empty\n");
        return;
    }

    printf("Stack: ");

    for (i = top; i >= 0; i--)
    {
        printf("%d ", stack[i]);
    }

    printf("\n");
}


/* =========================
   QUEUE
   ========================= */

int queue[MAX_SIZE];
int front = -1;
int rear = -1;

void enqueue(int value)
{
    if (rear == MAX_SIZE - 1)
    {
        printf("Queue Overflow\n");
        return;
    }

    if (front == -1)
    {
        front = 0;
    }

    rear++;
    queue[rear] = value;

    printf("Enqueued %d into Queue\n", value);
}

void dequeue(void)
{
    if (front == -1 || front > rear)
    {
        printf("Queue Underflow\n");
        return;
    }

    printf("Dequeued %d from Queue\n", queue[front]);
    front++;

    if (front > rear)
    {
        front = -1;
        rear = -1;
    }
}

void display_queue(void)
{
    int i;

    if (front == -1)
    {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue: ");

    for (i = front; i <= rear; i++)
    {
        printf("%d ", queue[i]);
    }

    printf("\n");
}


/* =========================
   MAIN PROGRAM
   ========================= */

int main(void)
{
    int choice;
    int value;

    while (1)
    {
        printf("\n===== STACK AND QUEUE =====\n");
        printf("1. Push to Stack\n");
        printf("2. Pop from Stack\n");
        printf("3. Display Stack\n");
        printf("4. Enqueue to Queue\n");
        printf("5. Dequeue from Queue\n");
        printf("6. Display Queue\n");
        printf("7. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                printf("Enter value: ");
                scanf("%d", &value);
                push(value);
                break;

            case 2:
                pop();
                break;

            case 3:
                display_stack();
                break;

            case 4:
                printf("Enter value: ");
                scanf("%d", &value);
                enqueue(value);
                break;

            case 5:
                dequeue();
                break;

            case 6:
                display_queue();
                break;

            case 7:
                printf("Program terminated.\n");
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}