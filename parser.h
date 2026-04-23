#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <bits/stdc++.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

using namespace std;

/** This file handles parsing the data we are going to work with **/
/** It also holds all the global variables we parse into         **/

string operation;
int last_instant, process_count;
vector<pair<char, int>> algorithms;
vector<tuple<string, int, int, int>> processes;
vector<vector<char>> timeline;
unordered_map<string, int> processToIndex;

// Results

vector<int> finishTime;
vector<int> turnAroundTime;
vector<float> normTurn;

// Detects whether input is being typed by a user or piped from a file/stream.
bool isInteractiveInput()
{
#ifdef _WIN32
    return _isatty(_fileno(stdin));
#else
    return isatty(fileno(stdin));
#endif
}

// Parses the comma-separated algorithm list and extracts RR quantum when present.
void parse_algorithms(string algorithm_chunk)
{
    stringstream stream(algorithm_chunk);
    while (stream.good())
    {
        string temp_str;
        getline(stream, temp_str, ',');
        stringstream ss(temp_str);
        getline(ss, temp_str, '-');
        char algorithm_id = temp_str[0];
        getline(ss, temp_str, '-');
        int quantum = temp_str.size() >= 1 ? stoi(temp_str) : -1;
        algorithms.push_back(make_pair(algorithm_id, quantum));
    }
}

// Reads all process definitions in either interactive or compact comma-separated format.
void parse_processes(bool interactive)
{
    string process_chunk, process_name;
    int process_arrival_time, process_burst_time, process_priority;
    for (int i = 0; i < process_count; i++)
    {
        if (interactive)
        {
            // Prompt the user for each field when the program is run manually.
            cout << "Process " << i + 1 << ":\n";
            cout << "  Enter process name: ";
            cin >> process_name;
            cout << "  Enter arrival time: ";
            cin >> process_arrival_time;
            cout << "  Enter burst time: ";
            cin >> process_burst_time;
            cout << "  Enter priority: ";
            cin >> process_priority;
        }
        else
        {
            // Expected format: name,arrival,burst,priority
            cin >> process_chunk;

            stringstream stream(process_chunk);
            string temp_str;
            getline(stream, temp_str, ',');
            process_name = temp_str;
            getline(stream, temp_str, ',');
            process_arrival_time = stoi(temp_str);
            getline(stream, temp_str, ',');
            process_burst_time = stoi(temp_str);
            getline(stream, temp_str, ',');
            process_priority = temp_str.empty() ? 0 : stoi(temp_str);
        }

        processes.push_back(make_tuple(process_name, process_arrival_time, process_burst_time, process_priority));
        processToIndex[process_name] = i;
    }
}

// Reads the full simulator configuration and prepares shared result containers.
void parse()
{
    string algorithm_chunk;
    bool interactive = isInteractiveInput();

    if (interactive)
        cout << "Enter mode (trace/stats): ";
    cin >> operation;

    if (interactive)
        cout << "Enter algorithms (1=FCFS, 2=SJF, 3=SRTF, 4=PRIORITY, 5-q=RR): ";
    cin >> algorithm_chunk;

    if (interactive)
        cout << "Enter last instant: ";
    cin >> last_instant;

    if (interactive)
        cout << "Enter number of processes: ";
    cin >> process_count;

    parse_algorithms(algorithm_chunk);
    parse_processes(interactive);

    // Pre-size result arrays so each algorithm can reuse the same storage.
    finishTime.resize(process_count);
    turnAroundTime.resize(process_count);
    normTurn.resize(process_count);
    timeline.resize(last_instant);

    // Initialize the timeline as blank cells for every time/process combination.
    for (int i = 0; i < last_instant; i++)
        for (int j = 0; j < process_count; j++)
            timeline[i].push_back(' ');
}

#endif // PARSER_H_INCLUDED
