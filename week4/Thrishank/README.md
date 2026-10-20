# Interactive Deadlock Simulation System — Member 2

## Process and Resource Management (C)

This module manages process and resource counts, resource instances, the Allocation Matrix, Maximum Matrix, Available Vector, and Need Matrix.

## Requirements

- Ubuntu / WSL
- GCC
- GNU Make
- Visual Studio Code (optional)

Install the build tools if needed:

```bash
sudo apt update
sudo apt install build-essential
```

## Project files

- `main.c` — console application and interactive input
- `resource_manager.h` — shared data structure and function declarations
- `resource_manager.c` — matrix management, validation, and display
- `test_resource_manager.c` — automated tests
- `Makefile` — build and test commands

## Build

Run these commands from the `member2` directory:

```bash
make clean
make
```

Run the application:

```bash
./member2
```

Run the automated tests:

```bash
make test
```

## Matrix calculations

For process `i` and resource type `j`:

**Need Matrix**

`Need[i][j] = Maximum[i][j] - Allocation[i][j]`

**Available Vector**

`Available[j] = Total[j] - sum(Allocation[i][j])`

The module rejects negative resource totals, negative matrix values, allocations exceeding maximum claims, and total allocations exceeding available resource inventory.

## Example

Given totals `10 5 7`, Allocation:

```text
P0: 0 1 0
P1: 2 0 0
P2: 3 0 2
```

and Maximum:

```text
P0: 7 5 3
P1: 3 2 2
P2: 9 0 2
```

the calculated Available Vector is:

```text
5 4 5
```

and the Need Matrix is:

```text
P0: 7 4 3
P1: 1 2 2
P2: 6 0 0
```

## Integration

### Member 3 — Banker's Algorithm

The algorithm can read `state.available`, `state.allocation`, `state.maximum`, and `state.need` from `RMState`.

It can use these arrays to check safe sequences and validate resource requests.

### Member 4 — Deadlock Detection and Resource Allocation Graph

The detection module can use `state.allocation` to identify allocated resources and `state.available` to inspect unallocated resource instances.

A resource-allocation graph requires additional graph data structures and logic; it is not implemented by this resource-management module.

## Automated tests

The test program checks valid configurations, matrix calculations, invalid dimensions, negative totals, excessive allocations, and process/resource indexes.

Expected result: 13 passing checks and zero failures.

## Limitations

- Maximum of 50 processes and 20 resource types.
- Console interface only.
- Resource requests, safety analysis, deadlock detection, and recovery belong to other modules.
- `resource_manager.c` provides the shared resource-management API; the existing `main.c` may need integration changes if it currently maintains a separate internal data structure.

## Team

Member 2 — Process and Resource Management

Project: Interactive Deadlock Simulation System
