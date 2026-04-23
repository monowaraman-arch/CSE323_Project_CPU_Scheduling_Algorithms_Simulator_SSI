# CPU Scheduling Algorithms Simulator

This project is a C++ console application that simulates and compares five CPU scheduling algorithms used in operating systems.

## Algorithms Implemented

- First-Come, First-Served (`FCFS`)
- Shortest Job First (`SJF`)
- Shortest Remaining Time First (`SRTF`)
- Priority Scheduling
- Round Robin (`RR`)

## Current Project Structure

The repository is organized with the main project files in a single folder:

- `main.cpp` - scheduling algorithm implementations and output formatting
- `parser.h` - input parsing and shared data structures
- `makefile` - simple build commands
- `README.md` - project documentation
- `PROJECT_PROPOSAL.pdf` - project proposal document
- `TECHNICAL_REPORT.docx` - technical report
- `CSE323_Project_Recording.mp4` - project demo recording

## Features

- Runs multiple scheduling algorithms in one execution
- Supports `trace` mode for timeline-based output
- Supports `stats` mode for performance metrics
- Displays finish time, turnaround time, and normalized turnaround time
- Supports Round Robin with custom quantum values such as `5-2`
- Accepts interactive input from the terminal

## Build

### Using `make`

```bash
make
```

This builds the program as `lab4`.

### Using `g++`

```bash
g++ -std=c++17 -o lab4 main.cpp
```

On Windows PowerShell:

```powershell
g++ -std=c++17 -o lab4.exe main.cpp
```

## How To Run

If you already have the executable:

```powershell
.\lab4.exe
```

If you build with `g++`:

```powershell
g++ -std=c++17 -o lab4.exe main.cpp
.\lab4.exe
```

## Input Format

When the program runs interactively, enter the values in this order.

### Line 1: Mode

```text
trace
```

or

```text
stats
```

### Line 2: Scheduling Algorithms

Enter one or more algorithms separated by commas.

```text
1,2,3,4,5-2
```

### Line 3: Last Instant

This is the final time value shown in the timeline.

```text
15
```

### Line 4: Number of Processes

```text
3
```

### Remaining Input: Process Data

For interactive mode, the program asks for each process separately:

- process name
- arrival time
- burst time
- priority

Example interactive entry:

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

## Algorithm IDs

- `1` = FCFS
- `2` = SJF
- `3` = SRTF
- `4` = Priority Scheduling
- `5-q` = Round Robin with quantum `q`

Examples:

- `1,2,3`
- `4,5-2`
- `1,2,3,4,5-4`

## Output

The simulator supports two output modes.

### `trace` Mode

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

### `stats` Mode

This mode prints:

- arrival time
- burst time
- finish time
- turnaround time
- normalized turnaround time

Example:

```text
SRTF
Process    |  P1  |  P2  |  P3  |
Arrival    |  0  |  1  |  2  |
Burst      |  5  |  3  |  1  | Mean|
Finish     |  9  |  5  |  3  |-----|
Turnaround |  9  |  4  |  1  | 4.67|
NormTurn   | 1.80| 1.33| 1.00| 1.38|
```

## Priority Rule

In this project:

- a larger priority value means higher priority
- if two processes have the same priority, the earlier arrival is chosen first
- if both priority and arrival time are the same, input order is used

## Notes

- Priority is most relevant for Priority Scheduling, but the program still reads it for every process
- The process list should be entered in arrival order
- `trace` mode is useful for visual comparison
- `stats` mode is useful for numerical comparison

## Included Documents

- `PROJECT_PROPOSAL.pdf`
- `TECHNICAL_REPORT.docx`
- `CSE323_Project_Recording.mp4`

## Summary

This project demonstrates operating systems concepts through a working simulation of CPU scheduling algorithms. It also shows practical use of C++ data structures such as queues, priority queues, vectors, and tuples.
