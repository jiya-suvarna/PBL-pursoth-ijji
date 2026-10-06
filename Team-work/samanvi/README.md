INTER-PROCESS COMMUNICATION (IPC)
PIPES AND MESSAGE QUEUES

PIPES :A pipe is a simple IPC mechanism that provides a communication channel between processes.
It allows one process to write data into the pipe and another process to read the data from it.
The basic idea is:
Process A → Pipe → Process B
A traditional pipe carries data as a stream of bytes.
The operating system manages the pipe and provides the processes with the required read and write operations.
Pipes are commonly used when processes need a simple way to transfer data.

Working of a Pipe :
The basic working of a pipe is:
Process A → Write → Pipe → Read → Process B
The steps are:
1. The operating system creates the pipe.
2. One process writes data into the pipe.
3. The pipe temporarily holds the data.
4. Another process reads the data from the pipe.
5. The data is consumed as it is read.
The pipe acts as a temporary communication channel between the processes.
For example, if Process A produces the data "Hello", it can write it into the pipe.
Process B can then read "Hello"from the pipe.

Anonymous Pipe:
An anonymous pipe is a pipe that does not have a name in the file system.
It is commonly used for communication between related processes, such as a parent process and its child process.
For example:
Parent Process → Anonymous Pipe → Child Process
It is useful when processes have a known relationship with each other.
Anonymous pipes are generally simple and are suitable for short and straightforward communication.

Named Pipe:
A named pipe is a pipe that has a name provided by the operating system.
Because it has a name, processes can use that name to access the communication channel.
Named pipes can be used for communication between unrelated processes, depending on the operating system.
For example:
Process A → Named Pipe → Process B
This makes named pipes more flexible than anonymous pipes for certain applications.
Example of Pipes
Consider a producer and consumer example.
The producer process generates data.
It writes the data into the pipe.
The consumer process reads the data from the pipe.
So the communication is:
Producer → Pipe → Consumer
A simple real-life example is a physical water pipe.
Water enters from one side and moves through the pipe to the other side.
Similarly, data is written into one end of a pipe and read from the other end.

Advantages of Pipes:
* Simple: Pipes are easy to understand and use.
* Easy data transfer:They provide a straightforward way to transfer data.
* Useful for related processes: Anonymous pipes are commonly used between related processes.
* Temporary communication: Data can be passed through the pipe as needed.
* Useful for connecting processes: The output of one process can be connected to the input of another process.

Limitations of Pipes:
* A traditional pipe provides a byte stream, rather than separate messages.
* Pipes are mainly used for local process communication.
* They are not always suitable for complex communication requirements.
* They may not be the best choice when processes need to exchange large or highly structured data.
* Other IPC mechanisms may be more suitable depending on the application.

Key Point:
Pipe = A communication channel used to transfer a stream of data between processes.

MESSAGE QUEUES:
A message queue is an IPC mechanism in which processes communicate by sending and receiving separate messages through a queue.
The sender places a message into the queue.
The receiver reads the message from the queue when it is ready.
The basic idea is:
Process A → Message Queue → Process B
Unlike a traditional pipe, a message queue keeps data as separate messages.

Working of a Message Queue:
The basic working is:
Sender → Message Queue → Receiver
The steps are:
1. The operating system creates or manages a message queue.
2. The sender creates a message.
3. The sender places the message into the queue.
4. The message remains in the queue until it is received.
5. The receiver reads the required message.
6. The message is removed according to the queue's operating rules.
This allows the sender and receiver to work without needing to communicate at exactly the same moment.

Why Use a Message Queue?
A message queue is useful when messages need to wait temporarily before the receiver processes them.
For example, suppose a server receives three requests:
Request 1 → Request 2 → Request 3
If the server is busy processing Request 1, the other messages can remain in the queue until the server is ready.
This helps separate the sender's activity from the receiver's activity.
Therefore, message queues are useful when processes do not need to operate at exactly the same time.

Message Types:
Message queues can support different types of messages depending on the operating system and implementation.
For example, an application may use:
* Request message
* Result message
* Error message
* Status message
A receiver can use the message type or other queue rules to determine which message should be processed.
This makes message queues useful for structured communication between processes.

Example of Message Queue :
Consider a server that receives requests from different processes.
The requests may arrive as:
Request 1 → Request 2 → Request 3
They are placed in the message queue.
The server processes the messages as required.
The basic flow is:
Client → Message Queue → Server
This is useful when many requests need to be handled by a process.

Advantages of Message Queues:
* Separate messages:Each message is treated as a separate unit.
* Messages can wait: A message can remain in the queue until the receiver processes it.
* Flexible communication:Sender and receiver do not always need to operate at exactly the same time.
* Multiple messages: Several messages can wait in the queue.
* OS managed:The operating system manages the queue and its operations.

Limitations of Message Queues :
* Message queues have system-dependent size limits.
* They can have more overhead than direct shared-memory communication.
* They may not be the best choice for very large amounts of continuously shared data.
* The exact ordering and message-selection rules depend on the operating system or implementation.

Key Point :
Message Queue = A queue used by processes to send and receive separate messages.

PIPE vs MESSAGE QUEUE:
*Data Form :
 Pipe: Transfers data as a byte stream.
 Message Queue: Transfers data as separate messages.
*Communication :
 Pipe: Stream-based communication.
 Message Queue: Message-based communication.
*Message Boundaries :
 Pipe: Normal byte streams do not preserve separate message boundaries.
 Message Queue: Each message remains a separate unit.
*Main Use :
 Pipe: Used for simple data transfer between processes.
 Message Queue: Used for structured message exchange between processes.
*Data Waiting :
 Pipe: Data remains in the pipe until it is read.
 Message Queue: Messages remain in the queue until they are received.
*Example :
 Pipe: Producer → Pipe → Consumer
 Message Queue: Sender → Message Queue → Receiver

Simple Difference to Remember :
Pipe → Stream of data
Message Queue → Separate messages
Message Queue → Separate messages
Therefore, a pipe is suitable when processes need a simple stream of data, while a message queue is useful when processes need to exchange distinct messages that can wait in a queue.
