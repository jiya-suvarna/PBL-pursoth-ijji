# PBL - Multi-Process Simulator & IPC

## Team: Pursoth-Ijji

## Overview

A C-based multi-process simulator developed as part of Problem Based Learning (PBL).

The project demonstrates process coordination and Inter-Process Communication (IPC) using POSIX Message Queues and Named FIFO on a POSIX/Linux environment.

The simulator is divided into three independent processes:

* **UI Process (`ui.c`)** – accepts user commands and displays simulator output.
* **Core Process (`core2.c`)** – performs CPU execution, memory management, stack operations and queue operations.
* **Logger Process** – handles logging messages and stores execution and error information.

The project also includes **performance benchmarking** to evaluate the behaviour and overhead of the multi-process system.

---

## System Architecture

```text
                         USER
                           |
                           v
                +----------------------+
                |      UI PROCESS      |
                |       ui.c           |
                |----------------------|
                | User Commands        |
                | Display Output       |
                +----------------------+
                           |
                           |
                    UI → Core
               POSIX Message Queue
                           |
                           v
                +----------------------+
                |     CORE PROCESS     |
                |      core2.c         |
                |----------------------|
                | CPU Execution        |
                | Memory Management    |
                | Stack Operations     |
                | Queue Operations     |
                +----------------------+
                           |
                           |
                   Core → Logger
               POSIX Message Queue
                           |
                           v
                +----------------------+
                |    LOGGER PROCESS    |
                |----------------------|
                | Message Handling     |
                | Event Logging        |
                | Error Logging        |
                +----------------------+
                           |
                           |
                      Named FIFO
                           |
                           v
                +----------------------+
                |      LOG FILES       |
                |----------------------|
                | execution.log        |
                | error.log            |
                +----------------------+
```

---

## IPC Layer

The project uses IPC mechanisms to allow the independent processes to communicate.

| Communication | IPC Mechanism       |
| ------------- | ------------------- |
| UI → Core     | POSIX Message Queue |
| Core → Logger | POSIX Message Queue |
| Logger → Logs | Named FIFO          |

### Why IPC?

IPC allows the UI, Core and Logger to operate as separate processes while exchanging commands, responses and logging information.

---

## Main Features

* Multi-process simulator
* CPU execution simulation
* Memory management
* Stack operations
* Queue operations
* Program loading and execution
* Run, stop and reset operations
* Inter-process communication
* Execution logging
* Error logging
* Named FIFO based log communication
* Performance benchmarking
* Testing and system integration

---

## Benchmarking

Benchmarking was added to evaluate the performance of the simulator.

The benchmark analysis focuses on:

* Execution time
* IPC overhead
* Multi-process performance
* System resource usage
* Comparison of execution behaviour

The benchmarking results help evaluate the performance of the multi-process architecture and communication between processes.

---

## Technologies Used

* **Language:** C
* **Platform:** POSIX/Linux
* **IPC:** POSIX Message Queues and Named FIFO
* **Interface:** Terminal
* **Logging:** File-based logging
* **Compilation:** GCC

---

## Team Responsibilities

| Member        | Responsibility                                       |
| ------------- | ---------------------------------------------------- |
| **Thrishank** | UI Process                                           |
| **Samaanvi**  | Stack and Queue Operations                           |
| **Sayyam**    | CPU and Memory                                       |
| **Diganth**   | Logger                                               |
| **Jiya**      | Team Lead – IPC, Integration, Testing & Benchmarking |

---

## Repository Structure

```text
PBL-pursoth-ijji/
│
├── WEEK2/
│   ├── ui.c
│   ├── core2.c
│   ├── logger.c
│   ├── ipc.c
│   ├── ipc.h
│   ├── execution.log
│   ├── error.log
│   └── benchmark files
│
└── README.md
```

---

## Execution Flow

```text
User Command
     ↓
UI Process
     ↓
POSIX Message Queue
     ↓
Core Process
     ↓
CPU / Memory / Stack / Queue
     ↓
POSIX Message Queue
     ↓
Logger Process
     ↓
Named FIFO
     ↓
Execution / Error Logs
```

---

## Project Outcome

The project demonstrates how a simulator can be divided into independent processes and connected using IPC mechanisms.

The final system provides:

* Independent process execution
* Communication between processes
* CPU and memory simulation
* Stack and queue operations
* Centralized logging
* Error handling
* Performance benchmarking
* Integrated multi-process execution

---

## Team

### Pursoth-Ijji

| Member        | Role                                                 |
| ------------- | ---------------------------------------------------- |
| **Thrishank** | UI                                                   |
| **Samaanvi**  | Stack & Queue                                        |
| **Sayyam**    | CPU & Memory                                         |
| **Diganth**   | Logger                                               |
| **Jiya**      | Team Lead – IPC, Integration, Testing & Benchmarking |

---

## Conclusion

The Multi-Process Simulator demonstrates the practical use of process management and IPC in C/POSIX systems.

By separating the UI, Core and Logger into independent processes, the project provides a modular architecture while demonstrating message-based communication, CPU and memory simulation, data-structure operations, logging, testing and performance benchmarking.
