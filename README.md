# CPU Scheduling Algorithms Simulator

This project is a C++ console application for simulating and comparing common CPU scheduling algorithms used in operating systems courses. It can display either an execution timeline or a statistics table for the selected algorithms, making it useful for both demonstration and analysis.

## Implemented Algorithms

- First Come First Serve (`FCFS`)
- Shortest Job First (`SJF`)
- Shortest Remaining Time First (`SRTF`)
- Priority Scheduling
- Round Robin (`RR`)

## Features

- Runs multiple scheduling algorithms in a single execution
- Supports `trace` mode for timeline-based visualization
- Supports `stats` mode for performance comparison
- Calculates finish time, turnaround time, and normalized turnaround time
- Supports Round Robin with configurable quantum values such as `5-2`
- Accepts both interactive terminal input and compact piped input

## Video Simulation

- [YouTube Simulation Video](https://youtu.be/lVJODH5qnPw)

## Project Files

- `main.cpp` - scheduling logic, execution flow, and output formatting
- `parser.h` - input parsing, shared data structures, and global containers
- `makefile` - simple build commands for GCC or MinGW-style environments
- `README.md` - project documentation
- `PROJECT_PROPOSAL.pdf` - project proposal
- `TECHNICAL_REPORT.pdf` - technical report

## Build

### With `make`

```bash
make target
```

This produces:

- `main.o`
- `lab4`

### With `g++`

Linux or macOS:

```bash
g++ -std=c++17 -o lab4 main.cpp
```

Windows PowerShell:

```powershell
g++ -std=c++17 -o lab4.exe main.cpp
```

## Run

If you built the Windows executable:

```powershell
.\lab4.exe
```

If you built on Linux or macOS:

```bash
./lab4
```

## Input Modes

The program automatically detects whether input is interactive or piped.

### Interactive Input

When you run the program directly from the terminal, it prompts for:

1. Mode: `trace` or `stats`
2. Algorithm list
3. Last instant
4. Number of processes
5. Process data for each process:
   - process name
   - arrival time
   - burst time
   - priority

Example:

```text
stats
1,2,3,4,5-2
15
3
P1
0
5
2
P2
1
3
4
P3
2
1
1
```

### Piped or File Input

When input is redirected from a file or stream, each process should be written in compact format:

```text
name,arrival,burst,priority
```

Example:

```text
trace
1,3,5-2
15
3
P1,0,5,2
P2,1,3,4
P3,2,1,1
```

## Algorithm Codes

- `1` = FCFS
- `2` = SJF
- `3` = SRTF
- `4` = Priority Scheduling
- `5-q` = Round Robin with quantum `q`

Examples:

- `1,2,3`
- `4,5-2`
- `1,2,3,4,5-4`

## Output Modes

### `trace`

This mode prints a scheduling timeline.

- `*` means the process is running
- `.` means the process is waiting
- blank space means the process has not arrived yet or has already finished

Example:

```text
RR-2  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
------------------------------------------------
P1     |*|*|.|.|.|*|*|.|*| | | | | | |
P2     | |.|*|*|.|.|.|*| | | | | | | |
P3     | | |.|.|*| | | | | | | | | | |
------------------------------------------------
```

### `stats`

This mode prints a summary table for each selected algorithm, including:

- arrival time
- burst time
- priority values for Priority Scheduling
- finish time
- turnaround time
- normalized turnaround time
- average turnaround and average normalized turnaround

Example:

```text
SRTF
Process    |  P1  |  P2  |  P3  |
Arrival    |  0  |  1  |  2  |
Burst    |  5  |  3  |  1  | Mean|
Finish     |  9  |  5  |  3  |-----|
Turnaround |  9  |  4  |  1  | 4.67|
NormTurn   | 1.80| 1.33| 1.00| 1.38|
```

## Scheduling Rules Used

- Higher numeric priority means higher scheduling priority
- If two processes have the same priority, the earlier arrival is chosen first
- If both priority and arrival time are the same, the earlier input order is used
- Processes should be entered in arrival order for correct simulation behavior

## Documents Included

- `PROJECT_PROPOSAL.pdf`
- `TECHNICAL_REPORT.pdf`

## Summary

This simulator demonstrates core operating systems scheduling concepts through a practical C++ implementation using queues, priority queues, vectors, and tuples. It is designed to help compare algorithm behavior visually and numerically in a straightforward console-based environment.
