INTER-PROCESS COMMUNICATION (IPC)

 SHARED MEMORY AND SEMAPHORES

  SHARED MEMORY

 Definition
 is an IPC mechanism in which two or more processes can access the same region of memory.
Normally, each process has its own separate memory space.
With shared memory, the operating system provides a common memory region that can be accessed by the processes that are allowed to use it.
The basic idea is:

Process A ↔ Shared Memory ↔ Process B
The processes communicate by reading and writing data in this common memory area.
Shared memory is mainly useful when processes need to exchange a large amount of data efficiently.

Working of Shared Memory
The basic working of shared memory is:
OS creates/provides shared region → Processes get access → Processes map the region → Data is shared

The steps are:

1. The operating system creates or provides a shared memory region.
2. The required processes are given access to the region.
3. The processes map the shared region into their address spaces.
4. One process can write data into the shared memory.
5. Another process can read the data from the same memory.
6. Both processes can communicate through the common memory area.

For example:
Process A → Write Data → Shared Memory → Read Data → Process B
The processes do not need to continuously send the same data through separate communication channels.

Example of Shared Memory
Imagine that two students are working on the same whiteboard.
Student A writes information on the whiteboard.
Student B can directly read the information from the same whiteboard.
Similarly, shared memory acts like a common memory area.

Process A writes → Common Memory → Process B reads

This makes it useful when processes need to share data frequently.


Why is Shared Memory Efficient?
Shared memory can be very efficient because, after the shared region is established, processes can directly access the common data.
It can reduce the need for repeated copying of data between separate process spaces.
This makes shared memory useful for applications that need to exchange large amounts of data between processes on the same system.
For example, two processes working with a large data buffer can use a shared memory region instead of repeatedly sending the entire data through messages.


Synchronization Problem in Shared Memory
One important problem with shared memory is synchronization.
Suppose Process A and Process B try to modify the same data at the same time.
This can produce incorrect or unexpected results.
This situation is related to a race condition.

For example:
Process A → Changes Data
Process B → Changes the Same Data
If both processes access the data without proper coordination, the final result may be incorrect.
Therefore, shared memory is usually used together with a synchronization mechanism such as a semaphore.

 Advantages of Shared Memory

Efficient for large data: Large amounts of data can be shared efficiently.
Direct access: Processes can directly access the common memory area.
Reduced copying: It can reduce repeated copying of data.
High-speed local communication: It is useful for communication between processes on the same system.
Useful for frequent data sharing: Processes can repeatedly access shared information.

 Limitations of Shared Memory

Synchronization is required when multiple processes access shared data.
Incorrect synchronization can cause race conditions.
Processes must carefully control access to shared data.
It is mainly useful for processes on the same system.
Programming becomes more complex when many processes access the same memory.

Key Point
Shared Memory = A common memory area that can be accessed by multiple processes for sharing data.


SEMAPHORES

Definition

A semaphore is a synchronization mechanism used to control access to shared resources and coordinate processes.
Its main purpose is synchronization and access control, not transferring data.
A semaphore helps make sure that processes access a shared resource in a controlled manner.

The basic idea is:
Process → Semaphore → Shared Resource


Why are Semaphores Needed?
Suppose several processes want to use the same printer.
If all processes try to use the printer at the same time, their operations may conflict.
A semaphore can control access to the printer.

For example:
Process A → Printer
While Process A is using the printer, Processes B and C wait.
After Process A finishes, another process can use the printer.
Therefore, a semaphore helps prevent conflicts when processes share resources.


Basic Operations of Semaphore
The two basic conceptual operations are:

Wait

The wait operation is performed when a process wants to access a resource.
If the resource is available, the process can continue.
If the resource is not available, the process may have to wait.
It is also commonly called P, down, or similar names depending on the system or textbook.

Signal

The signal operation is performed when a process finishes using the protected resource.
It indicates that the resource can be made available to another waiting process.
It is also commonly called V, up, or similar names depending on the system or textbook.

Simple idea:
Wait → Use Resource → Signal

Types of Semaphores
There are two commonly discussed types of semaphores:

Binary Semaphore
A binary semaphore has two possible states and is commonly used to control access to a resource.
It can represent situations such as:
Available / Not Available
For example, it can be used when only one process should enter a particular protected section at a time.

Counting Semaphore
A counting semaphore can have a range of values.
It is useful when there are multiple identical resources available.
For example, suppose a system has 5 printers.

A counting semaphore can keep track of the number of printers currently available.
5 printers → Semaphore count = 5
As processes use printers, the count can decrease.
When printers are released, the count can increase.

Semaphore Example
Consider a computer system with one printer.
Three processes want to use it:
Process A → Printer
Process B → Printer
Process C → Printer

The semaphore controls access.
Process A gets access to the printer first.
Processes B and C wait.
After Process A finishes, it performs the signal operation.
Then another waiting process can access the printer.
This prevents multiple processes from trying to use the same resource at the same time.

 Advantages of Semaphores

Prevents conflicts: Helps prevent simultaneous conflicting access.
Provides synchronization: Coordinates the execution of processes.
Controls shared resources: Helps manage access to resources.
Useful with shared memory: Can control access to shared data.
Supports multiple resources:Counting semaphores can manage several identical resources.

 Limitations of Semaphores

Incorrect use can lead to deadlock.
Processes may have to wait for resources.
Proper synchronization logic is required.
Incorrect handling can cause synchronization problems.
Programs using many semaphores can become difficult to manage.

Key Point
Semaphore = A synchronization mechanism used to control access to shared resources and coordinate processes.

SHARED MEMORY + SEMAPHORE
Shared memory and semaphores are often used together.
Shared Memory → Shares the Data
Semaphore → Controls Access to the Data

For example:
Process A ↔ Shared Memory ↔ Process B
A semaphore controls which process can access or modify the shared data at a particular time.
Therefore, shared memory provides the communication/data-sharing part, while the semaphore provides the synchronization part.


SHARED MEMORY vs SEMAPHORE
Shared Memory

Used to share data between processes.
Provides a common memory area.
Processes can read and write data directly.
Useful for fast data transfer.
Suitable for large amounts of data.
Example: Sharing a common buffer.
Semaphore

Used for process synchronization.
Provides a signaling mechanism.
Controls access to shared resources.
Uses wait() and signal() operations.
Helps prevent simultaneous access to a resource.
Example: Controlling access to a printer.

Therefore, shared memory allows processes to communicate through a common memory area, while semaphores make sure that the shared resource is accessed in a controlled and synchronized manner.
