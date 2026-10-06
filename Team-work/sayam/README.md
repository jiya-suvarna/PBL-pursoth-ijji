SIGNALS, SOCKETS AND RPC

1. SIGNALS

Definition

A signal is a notification sent by the operating system to a process when a particular event occurs.

Signals are mainly used for notification and process control rather than transferring large amounts of data.

Working

- An event occurs.
- The operating system sends a signal to the process.
- The process receives the signal.
- The process performs the appropriate action.

Examples of Signals

SIGINT: Used to interrupt a process. It is commonly generated when the user presses Ctrl + C.

SIGTERM: Requests a process to terminate normally. The process can perform necessary cleanup before terminating.

SIGKILL: Forces a process to terminate. Unlike SIGTERM, the process cannot catch or handle SIGKILL.

Example

When a program is running in a terminal and the user presses Ctrl + C, the operating system can send SIGINT to the process. The process can then respond to the interruption.

Advantages

- Simple method of notifying a process.
- Useful for process control.
- Useful for handling events.
- Requires very little information to be transferred.

Limitations

- Not suitable for transferring large amounts of data.
- Mainly used for notification.
- The process must handle the signal appropriately.

2. SOCKETS

Definition

A socket is a communication endpoint that allows processes to exchange data.

Sockets are commonly used for communication between processes on different computers through a network. They can also be used for communication between processes on the same computer.

Working

- The client creates or establishes communication with the server.
- The client sends a request through a socket.
- The server receives and processes the request.
- The server sends the response back through the socket.
- The client receives the response.

Client and Server

Client: A process that requests a service from another process.

Server: A process that provides the requested service.

For example, when a web browser requests a webpage, the browser acts as the client and the web server provides the webpage.

Types of Sockets

1. Stream Socket

- Provides a continuous stream of data.
- Commonly associated with TCP.
- Provides reliable and ordered communication.

2. Datagram Socket

- Sends individual packets called datagrams.
- Commonly associated with UDP.
- Does not provide the same delivery guarantees as TCP.

Uses of Sockets

- Web applications.
- Online communication.
- Client-server applications.
- Network-based applications.
- Communication between processes on different computers.

Advantages

- Supports communication between different computers.
- Useful for network applications.
- Supports client-server communication.
- Widely used in Internet applications.

Limitations

- Network delays may occur.
- Network failures can affect communication.
- Requires more setup than simple local IPC methods.

3. REMOTE PROCEDURE CALL (RPC)

Definition

Remote Procedure Call (RPC) is a communication mechanism that allows a program to request a function or procedure to execute in another process or on another computer.

It makes a remote operation work similar to a normal function call.

Working

- The client requests a remote procedure.
- The RPC system prepares the request.
- The request is sent to the server.
- The server receives the request.
- The server executes the required procedure.
- The result is sent back to the client.
- The client receives the result.

Example

Suppose a student application needs student information stored on a server.

The application sends a request to the server using RPC. The server processes the request and sends the required student information back to the application.

Why is it called Remote?

It is called remote because the requested procedure is executed outside the local process, often on another computer.

Advantages

- Makes communication between distributed systems easier.
- Allows remote services to be used like function calls.
- Hides many communication details from the programmer.
- Useful in client-server applications.
- Useful in distributed systems.

Limitations

- Network delays can affect performance.
- Network failures can interrupt communication.
- Remote calls require communication between the client and server.
- Performance may be slower than a normal local function call because communication is involved.
