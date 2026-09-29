# RPC, Memory-Mapped Files and Comparison

## 1. Remote Procedure Call (RPC)

**Remote Procedure Call (RPC)** is a communication method where a program requests a function to run in another process or computer.

### Working

**Client → Request → Server → Procedure Executed → Result → Client**

### Example

A client requests student details stored on a server. The server processes the request and sends the result back.

It is called **Remote** because the function runs outside the local process.

### Advantages

- Makes communication between distributed systems easier.
- Hides communication details from the programmer.
- Useful in client-server applications.

### Limitations

- Depends on network communication.
- Network delay or failure can affect the request.

> **Key Point:** RPC = Calling a function that runs on another system.

---

## 2. Memory-Mapped Files

A **Memory-Mapped File** is a file mapped into the virtual memory of a process. The process can access the file data like normal memory.

### Working

**File → Memory Mapping → Process Memory → Access Data**

### Example

A large file containing student records can be mapped into memory so that the process can access the required data efficiently.

Multiple processes can also map the same file and share its data.

### Advantages

- Useful for large files.
- Provides efficient access to file data.
- Can be used for sharing data between processes.

### Limitations

- Shared modifications must be handled carefully.
- Performance depends on memory and operating-system behavior.

> **Key Point:** Memory-Mapped File = Accessing file data through memory.

---

## 3. Quick Comparison of IPC Methods

| IPC Method | Main Purpose |
|---|---|
| **Pipe** | Transfers a stream of data |
| **Message Queue** | Transfers separate messages |
| **Shared Memory** | Shares a common memory area |
| **Semaphore** | Controls access to shared resources |
| **Signal** | Notifies a process about an event |
| **Socket** | Connects communication endpoints |
| **RPC** | Requests a function to run remotely |
| **Memory-Mapped File** | Allows file data to be accessed through memory |

### Easy Way to Remember

| Method | Remember As |
|---|---|
| **Pipe** | Stream |
| **Message Queue** | Messages |
| **Shared Memory** | Shared Data |
| **Semaphore** | Control |
| **Signal** | Notification |
| **Socket** | Network Communication |
| **RPC** | Remote Function |
| **Memory-Mapped File** | File as Memory |

---

## 4. Conclusion

**Inter-Process Communication (IPC)** helps processes **communicate, share data, and coordinate** their activities.

Different IPC methods are used for different requirements. The choice depends on:

- Type of data
- Amount of data
- Synchronization requirements
- Whether processes are on the same or different systems
