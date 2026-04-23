#include <bits/stdc++.h>
#include "parser.h"

using namespace std;

/** Global Constants **/
const string TRACE = "trace";
const string SHOW_STATISTICS = "stats";
const string ALGORITHMS[6] = {"", "FCFS", "SJF", "SRTF", "PRIORITY", "RR-"};

// Returns the display name stored in a process tuple.
string getProcessName(tuple<string, int, int, int> &process)
{
    return get<0>(process);
}

// Returns the time at which a process becomes ready.
int getArrivalTime(tuple<string, int, int, int> &process)
{
    return get<1>(process);
}

// Returns the total CPU time required by a process.
int getBurstTime(tuple<string, int, int, int> &process)
{
    return get<2>(process);
}

// Returns the priority value used by the priority scheduler.
int getPriorityLevel(tuple<string, int, int, int> &process)
{
    return get<3>(process);
}

// Resets the timeline and computed metrics before running the next algorithm.
void clear_timeline()
{
    for (int i = 0; i < last_instant; i++)
        for (int j = 0; j < process_count; j++)
            timeline[i][j] = ' ';

    fill(finishTime.begin(), finishTime.end(), 0);
    fill(turnAroundTime.begin(), turnAroundTime.end(), 0);
    fill(normTurn.begin(), normTurn.end(), 0.0f);
}

// Marks every non-running slot between arrival and completion as waiting time.
void fillInWaitTime()
{
    for (int i = 0; i < process_count; i++)
    {
        int arrivalTime = getArrivalTime(processes[i]);
        for (int time = arrivalTime; time < finishTime[i]; time++)
        {
            if (timeline[time][i] != '*')
                timeline[time][i] = '.';
        }
    }
}

// Stores finish, turnaround, and normalized turnaround for one process.
void updateProcessMetrics(int processIndex, int finish)
{
    int arrivalTime = getArrivalTime(processes[processIndex]);
    int BurstTime = getBurstTime(processes[processIndex]);
    finishTime[processIndex] = finish;
    turnAroundTime[processIndex] = finish - arrivalTime;
    normTurn[processIndex] = turnAroundTime[processIndex] * 1.0f / BurstTime;
}

// Simulates First Come First Serve by running processes in input order.
void firstComeFirstServe()
{
    int time = getArrivalTime(processes[0]);
    for (int processIndex = 0; processIndex < process_count; processIndex++)
    {
        int arrivalTime = getArrivalTime(processes[processIndex]);
        int BurstTime = getBurstTime(processes[processIndex]);

        time = max(time, arrivalTime);
        for (int current = arrivalTime; current < time; current++)
            timeline[current][processIndex] = '.';
        for (int current = time; current < time + BurstTime; current++)
            timeline[current][processIndex] = '*';

        time += BurstTime;
        updateProcessMetrics(processIndex, time);
    }
}

// Simulates non-preemptive Shortest Job First using burst time as the key.
void shortestJobFirst()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> readyQueue;
    int nextProcess = 0;

    for (int time = 0; time < last_instant; time++)
    {
        // Add every process that has arrived by the current time.
        while (nextProcess < process_count && getArrivalTime(processes[nextProcess]) <= time)
        {
            readyQueue.push(make_pair(getBurstTime(processes[nextProcess]), nextProcess));
            nextProcess++;
        }

        if (readyQueue.empty())
            continue;

        int processIndex = readyQueue.top().second;
        readyQueue.pop();

        int arrivalTime = getArrivalTime(processes[processIndex]);
        int BurstTime = getBurstTime(processes[processIndex]);

        // Everything before the execution window is waiting time for this process.
        for (int current = arrivalTime; current < time; current++)
            timeline[current][processIndex] = '.';
        for (int current = time; current < time + BurstTime; current++)
            timeline[current][processIndex] = '*';

        time += BurstTime - 1;
        updateProcessMetrics(processIndex, time + 1);
    }
}

// Simulates preemptive SJF by always running the job with least remaining time.
void shortestRemainingTimeFirst()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> readyQueue;
    int nextProcess = 0;

    for (int time = 0; time < last_instant; time++)
    {
        // Only jobs arriving at this exact instant are new candidates.
        while (nextProcess < process_count && getArrivalTime(processes[nextProcess]) == time)
        {
            readyQueue.push(make_pair(getBurstTime(processes[nextProcess]), nextProcess));
            nextProcess++;
        }

        if (readyQueue.empty())
            continue;

        int processIndex = readyQueue.top().second;
        int remainingTime = readyQueue.top().first;
        readyQueue.pop();

        // Execute the selected process for one time unit.
        timeline[time][processIndex] = '*';

        if (remainingTime == 1)
        {
            updateProcessMetrics(processIndex, time + 1);
        }
        else
        {
            // Put the process back with one less unit remaining.
            readyQueue.push(make_pair(remainingTime - 1, processIndex));
        }
    }

    fillInWaitTime();
}

// Simulates non-preemptive priority scheduling with arrival and input order tie-breakers.
void priorityScheduling()
{
    priority_queue<tuple<int, int, int>> readyQueue;
    int nextProcess = 0;

    for (int time = 0; time < last_instant; time++)
    {
        // Store priority first, then earlier arrival, then earlier input order.
        while (nextProcess < process_count && getArrivalTime(processes[nextProcess]) <= time)
        {
            readyQueue.push(make_tuple(
                getPriorityLevel(processes[nextProcess]),
                -getArrivalTime(processes[nextProcess]),
                -nextProcess));
            nextProcess++;
        }

        if (readyQueue.empty())
            continue;

        int processIndex = -get<2>(readyQueue.top());
        readyQueue.pop();

        int arrivalTime = getArrivalTime(processes[processIndex]);
        int BurstTime = getBurstTime(processes[processIndex]);

        for (int current = arrivalTime; current < time; current++)
            timeline[current][processIndex] = '.';
        for (int current = time; current < time + BurstTime; current++)
            timeline[current][processIndex] = '*';

        time += BurstTime - 1;
        updateProcessMetrics(processIndex, time + 1);
    }
}

// Simulates Round Robin using the provided quantum for time slicing.
void roundRobin(int originalQuantum)
{
    if (originalQuantum <= 0)
        return;

    queue<pair<int, int>> readyQueue;
    int nextProcess = 0;

    // Seed the ready queue if the first process is available at time zero.
    if (getArrivalTime(processes[nextProcess]) == 0)
    {
        readyQueue.push(make_pair(nextProcess, getBurstTime(processes[nextProcess])));
        nextProcess++;
    }

    int currentQuantum = originalQuantum;

    for (int time = 0; time < last_instant; time++)
    {
        if (!readyQueue.empty())
        {
            int processIndex = readyQueue.front().first;
            readyQueue.front().second--;
            int remainingBurstTime = readyQueue.front().second;

            currentQuantum--;
            timeline[time][processIndex] = '*';

            // New arrivals are appended to the queue before quantum completion is handled.
            while (nextProcess < process_count && getArrivalTime(processes[nextProcess]) == time + 1)
            {
                readyQueue.push(make_pair(nextProcess, getBurstTime(processes[nextProcess])));
                nextProcess++;
            }

            // Process finishes exactly as its time slice ends.
            if (currentQuantum == 0 && remainingBurstTime == 0)
            {
                updateProcessMetrics(processIndex, time + 1);
                currentQuantum = originalQuantum;
                readyQueue.pop();
            }
            // Time slice ends, so move the unfinished process to the back of the queue.
            else if (currentQuantum == 0 && remainingBurstTime != 0)
            {
                readyQueue.pop();
                readyQueue.push(make_pair(processIndex, remainingBurstTime));
                currentQuantum = originalQuantum;
            }
            // Process finishes before the current time slice is exhausted.
            else if (currentQuantum != 0 && remainingBurstTime == 0)
            {
                updateProcessMetrics(processIndex, time + 1);
                readyQueue.pop();
                currentQuantum = originalQuantum;
            }
        }

        // If the CPU was idle, we still need to capture jobs arriving for the next tick.
        while (nextProcess < process_count && getArrivalTime(processes[nextProcess]) == time + 1)
        {
            readyQueue.push(make_pair(nextProcess, getBurstTime(processes[nextProcess])));
            nextProcess++;
        }
    }

    fillInWaitTime();
}

// Prints the label of the algorithm currently being reported.
void printAlgorithm(int algorithmIndex)
{
    int algorithmId = algorithms[algorithmIndex].first - '0';
    if (algorithmId == 5)
        cout << ALGORITHMS[algorithmId] << algorithms[algorithmIndex].second << endl;
    else
        cout << ALGORITHMS[algorithmId] << endl;
}

// Prints the process names as the header row for statistics output.
void printProcesses()
{
    cout << "Process    ";
    for (int i = 0; i < process_count; i++)
        cout << "|  " << getProcessName(processes[i]) << "  ";
    cout << "|\n";
}

// Prints arrival times for every process.
void printArrivalTime()
{
    cout << "Arrival    ";
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ", getArrivalTime(processes[i]));
    cout << "|\n";
}

// Prints burst times and reserves the last column for the mean values below.
void printBurstTime()
{
    cout << "Burst    |";
    for (int i = 0; i < process_count; i++)
        printf("%3d  |", getBurstTime(processes[i]));
    cout << " Mean|\n";
}

// Prints the completion time of each process.
void printFinishTime()
{
    cout << "Finish     ";
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ", finishTime[i]);
    cout << "|-----|\n";
}

// Prints turnaround times and their average.
void printTurnAroundTime()
{
    cout << "Turnaround |";
    int sum = 0;
    for (int i = 0; i < process_count; i++)
    {
        printf("%3d  |", turnAroundTime[i]);
        sum += turnAroundTime[i];
    }
    if ((1.0 * sum / turnAroundTime.size()) >= 10)
        printf("%2.2f|\n", (1.0 * sum / turnAroundTime.size()));
    else
        printf(" %2.2f|\n", (1.0 * sum / turnAroundTime.size()));
}

// Prints normalized turnaround times and their average.
void printNormTurn()
{
    cout << "NormTurn   |";
    float sum = 0;
    for (int i = 0; i < process_count; i++)
    {
        if (normTurn[i] >= 10)
            printf("%2.2f|", normTurn[i]);
        else
            printf(" %2.2f|", normTurn[i]);
        sum += normTurn[i];
    }

    if ((1.0 * sum / normTurn.size()) >= 10)
        printf("%2.2f|\n", (1.0 * sum / normTurn.size()));
    else
        printf(" %2.2f|\n", (1.0 * sum / normTurn.size()));
}

// Prints process priorities when the priority scheduler is selected.
void printPriorityValues()
{
    cout << "Priority   ";
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ", getPriorityLevel(processes[i]));
    cout << "|\n";
}

// Prints the full statistics table for one algorithm run.
void printStats(int algorithmIndex)
{
    printAlgorithm(algorithmIndex);
    printProcesses();
    printArrivalTime();
    printBurstTime();
    if (algorithms[algorithmIndex].first == '4')
        printPriorityValues();
    printFinishTime();
    printTurnAroundTime();
    printNormTurn();
}

// Prints a per-process execution timeline using '*' for running and '.' for waiting.
void printTimeline(int)
{
    for (int i = 0; i <= last_instant; i++)
        cout << i % 10 << " ";
    cout << "\n";
    cout << "------------------------------------------------\n";
    for (int i = 0; i < process_count; i++)
    {
        cout << getProcessName(processes[i]) << "     |";
        for (int j = 0; j < last_instant; j++)
            cout << timeline[j][i] << "|";
        cout << " \n";
    }
    cout << "------------------------------------------------\n";
}

// Dispatches to the selected scheduling algorithm and prints its trace label when needed.
void execute_algorithm(char algorithmId, int quantum, string operation)
{
    switch (algorithmId)
    {
    case '1':
        if (operation == TRACE)
            cout << "FCFS  ";
        firstComeFirstServe();
        break;
    case '2':
        if (operation == TRACE)
            cout << "SJF   ";
        shortestJobFirst();
        break;
    case '3':
        if (operation == TRACE)
            cout << "SRTF  ";
        shortestRemainingTimeFirst();
        break;
    case '4':
        if (operation == TRACE)
            cout << "PRIO  ";
        priorityScheduling();
        break;
    case '5':
        if (operation == TRACE)
            cout << "RR-" << quantum << "  ";
        roundRobin(quantum);
        break;
    default:
        break;
    }
}

// Parses input, runs every requested algorithm, and prints the chosen output format.
int main()
{
    parse();
    for (int idx = 0; idx < (int)algorithms.size(); idx++)
    {
        // Each algorithm should start from the same clean state.
        clear_timeline();
        execute_algorithm(algorithms[idx].first, algorithms[idx].second, operation);
        if (operation == TRACE)
            printTimeline(idx);
        else if (operation == SHOW_STATISTICS)
            printStats(idx);
        cout << "\n";
    }
    return 0;
}
