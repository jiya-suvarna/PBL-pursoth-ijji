# PBL - Multi-Process Simulator & IPC

## Team: Pursoth-Ijji

## Overview

A C-based multi-process simulator developed as part of Problem Based Learning (PBL).

The project demonstrates process coordination and Inter-Process Communication (IPC) using POSIX Message Queues and Named FIFO on a POSIX/Linux environment.

The simulator is divided into three independent processes:

- **UI Process (`UI_Thrishank/ui.c`)** – accepts user commands and displays simulator output.
- **Core Process (`CORE.c`)** – performs CPU execution, memory management, stack operations and queue operations.
- **Logger Process (`logger.c`)** – receives and handles logging messages from the Core process.

The project also includes performance benchmarking to compare the single-process and multi-process versions.

---

## System Architecture

```text
                    USER
                      |
                      v
             +------------------+
             |    UI PROCESS    |
             | UI_Thrishank/ui.c|
             +------------------+
                      |
                      |
             POSIX Message Queue
                      |
                      v
             +------------------+
             |   CORE PROCESS   |
             |     CORE.c       |
             |------------------|
             | CPU Execution    |
             | Memory Management|
             | Stack Operations |
             | Queue Operations |
             +------------------+
                      |
                      |
                 Named FIFO
                      |
                      v
             +------------------+
             | LOGGER PROCESS   |
             |    logger.c      |
             +------------------+
                      |
                      v
                Log Messages
```

---

## IPC Layer

The project uses two IPC mechanisms to allow independent processes to communicate.

| Communication | IPC Mechanism |
|---|---|
| UI ↔ Core | POSIX Message Queues |
| Core → Logger | Named FIFO |

### Why IPC?

IPC allows the UI, Core and Logger to operate as separate processes while exchanging commands, responses and logging information.

- **POSIX Message Queues:** Used for communication between the UI and Core processes.
- **Named FIFO:** Used to send log messages from the Core process to the Logger process.

---

## Main Features

- Multi-process simulator
- CPU execution simulation
- Memory management
- Stack operations
- Queue operations
- Program loading and execution
- Run, stop and reset operations
- Inter-process communication
- Execution logging
- Named FIFO-based log communication
- Performance benchmarking
- Testing and system integration

---

## Benchmarking

Benchmarking was performed to evaluate the single-process and multi-process versions using repeated runs.

### Single-Process Benchmark Results

| Metric | Run 1 | Run 2 | Run 3 |
|---|---:|---:|---:|
| Execution Time | 0.01 s | 0.00 s | 0.01 s |
| CPU Usage | 8% | 11% | 9% |
| Maximum Memory (RSS) | 1916 KB | 1916 KB | 1916 KB |

**Average Results:**

- Execution Time: Approximately 0.007 seconds
- CPU Usage: Approximately 9.3%
- Maximum Memory (RSS): 1916 KB

### Multi-Process Benchmark Results

| Metric | Run 1 | Run 2 | Run 3 |
|---|---:|---:|---:|
| Elapsed Time | 2.24 s | 2.18 s | 2.18 s |
| CPU Usage | 1% | 1% | 1% |
| Maximum Memory (RSS) | 8276 KB | 8300 KB | 8256 KB |

**Average Results:**

- Elapsed Time: Approximately 2.20 seconds
- CPU Usage: Approximately 1%
- Maximum Memory (RSS): Approximately 8277 KB

### Performance Analysis

- The single-process version used less memory because all operations ran within one process.
- The multi-process version required additional memory for the separate UI, Core and Logger processes.
- The multi-process benchmark showed approximately 1% CPU usage.
- The multi-process elapsed time includes the two seconds of intentional startup delays used to start Logger and Core in the correct order.
- Therefore, the elapsed times are not a direct measurement of IPC overhead.
- IPC overhead was not measured separately.

---

## Technologies Used

- **Language:** C
- **Platform:** POSIX/Linux
- **IPC:** POSIX Message Queues and Named FIFO
- **Interface:** Terminal
- **Compilation:** GCC
- **Benchmarking:** Linux `/usr/bin/time -v`

---

## Team Responsibilities

| Member | Responsibility |
|---|---|
| **Thrishank** | UI Process |
| **Samaanvi** | Stack and Queue Operations |
| **Sayyam** | CPU and Memory |
| **Diganth** | Logger |
| **Jiya** | Team Lead – IPC, Integration, Testing and Benchmarking |

---

## Repository Structure

```text
PBL-pursoth-ijji/
│
├── WEEK2/
│   ├── CORE.c
│   ├── UI_Thrishank/
│   │   └── ui.c
│   ├── logger.c
│   ├── ipc.c
│   ├── ipc.h
│   ├── program.txt
│   ├── readme.md
│   └── benchmarks/
│       └── single_process.c
│
└── README.md
```

---

## Execution Flow

```text
User Command
     |
     v
 UI Process
     |
     v
POSIX Message Queue
     |
     v
 Core Process
     |
     v
CPU / Memory / Stack / Queue
     |
     v
   Named FIFO
     |
     v
Logger Process
     |
     v
 Log Messages
```

---

## Project Outcome

The project demonstrates how a simulator can be divided into independent processes and connected using IPC mechanisms.

The final system provides:

- Independent process execution
- Communication between processes
- CPU and memory simulation
- Stack and queue operations
- Centralized logging
- Integrated multi-process execution
- Performance benchmarking

---

## Team

### Pursoth-Ijji

| Member | Role |
|---|---|
| **Thrishank** | UI |
| **Samaanvi** | Stack & Queue |
| **Sayyam** | CPU & Memory |
| **Diganth** | Logger |
| **Jiya** | Team Lead – IPC, Integration, Testing & Benchmarking |

---

## Conclusion

The Multi-Process Simulator demonstrates the practical use of process management and IPC in C/POSIX systems.

By separating the UI, Core and Logger into independent processes, the project provides a modular architecture using POSIX Message Queues and Named FIFO. It also demonstrates CPU and memory simulation, data-structure operations, logging, integration, testing and performance benchmarking.
