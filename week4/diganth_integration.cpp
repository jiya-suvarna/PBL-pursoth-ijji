#include "diganth_integration.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;


struct DetectionResult {
    vector<bool> finished;
    vector<int> deadlockedProcesses;
    vector<int> completionOrder;
};

// Display a vector
void printVector(const vector<int>& values, const string& label) {
    cout << label << "[ ";
    for (int value : values)
        cout << value << " ";
    cout << "]\n";
}

// Display a matrix
void printMatrix(const vector<vector<int>>& matrix,
                 const string& title,
                 int n, int m) {
    cout << "\n" << title << "\n";
    cout << "Process";

    for (int j = 0; j < m; j++)
        cout << setw(6) << ("R" + to_string(j + 1));

    cout << '\n';

    for (int i = 0; i < n; i++) {
        cout << setw(7) << ("P" + to_string(i + 1));

        for (int j = 0; j < m; j++)
            cout << setw(6) << matrix[i][j];

        cout << '\n';
    }
}

// Deadlock detection algorithm
DetectionResult detectDeadlock(
    const vector<vector<int>>& allocation,
    const vector<vector<int>>& request,
    const vector<int>& available) {

    int n = allocation.size();
    int m = available.size();

    vector<int> work = available;
    vector<bool> finished(n, false);
    vector<int> completionOrder;

    // Processes holding no resources can be considered finished
    for (int i = 0; i < n; i++) {
        bool holdsResource = false;

        for (int j = 0; j < m; j++) {
            if (allocation[i][j] > 0) {
                holdsResource = true;
                break;
            }
        }

        if (!holdsResource)
            finished[i] = true;
    }

    bool progress = true;

    while (progress) {
        progress = false;

        for (int i = 0; i < n; i++) {
            if (finished[i])
                continue;

            bool canFinish = true;

            // Check whether available resources satisfy the request
            for (int j = 0; j < m; j++) {
                if (request[i][j] > work[j]) {
                    canFinish = false;
                    break;
                }
            }

            if (canFinish) {
                // Assume the process finishes and releases its resources
                for (int j = 0; j < m; j++)
                    work[j] += allocation[i][j];

                finished[i] = true;
                completionOrder.push_back(i);
                progress = true;
            }
        }
    }

    vector<int> deadlockedProcesses;

    // Unfinished processes are identified as deadlocked
    for (int i = 0; i < n; i++) {
        if (!finished[i])
            deadlockedProcesses.push_back(i);
    }

    return {finished, deadlockedProcesses, completionOrder};
}

// Generate the Resource Allocation Graph in Graphviz format
bool writeRAG(
    const vector<vector<int>>& allocation,
    const vector<vector<int>>& request,
    int n, int m,
    const vector<int>& deadlockedProcesses) {

    ofstream file("rag.dot");

    if (!file)
        return false;

    vector<bool> deadlocked(n, false);

    for (int p : deadlockedProcesses)
        deadlocked[p] = true;

    file << "digraph RAG {\n";
    file << "rankdir=LR;\n";
    file << "label=\"Resource Allocation Graph\";\n";
    file << "labelloc=t;\n";

    // Draw process nodes
    for (int i = 0; i < n; i++) {
        file << "P" << i + 1
             << " [shape=circle, label=\"P" << i + 1 << "\"";

        if (deadlocked[i])
            file << ", style=filled, fillcolor=lightcoral";
        else
            file << ", style=filled, fillcolor=lightblue";

        file << "];\n";
    }

    // Draw resource nodes
    for (int j = 0; j < m; j++) {
        file << "R" << j + 1
             << " [shape=box, label=\"R" << j + 1
             << "\", style=filled, fillcolor=lightyellow];\n";
    }

    // Draw allocation and request edges
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {

            // Resource to process: allocated resource
            if (allocation[i][j] > 0) {
                file << "R" << j + 1 << " -> P" << i + 1
                     << " [label=\"Allocated: "
                     << allocation[i][j]
                     << "\", color=green];\n";
            }

            // Process to resource: outstanding request
            if (request[i][j] > 0) {
                file << "P" << i + 1 << " -> R" << j + 1
                     << " [label=\"Request: "
                     << request[i][j]
                     << "\", color=orange];\n";
            }
        }
    }

    file << "}\n";
    file.close();

    return true;
}
extern "C" int run_deadlock_detection_from_gui(
    int p,
    int r,
    int gui_allocation[20][20],
    int gui_request[20][20],
    int gui_available[20],
    int deadlockedProcesses[20],
    int completionOrder[20],
    int *deadlockedCount,
    int *completionCount)
{
    vector<vector<int>> allocation(
        p, vector<int>(r));

    vector<vector<int>> request(
        p, vector<int>(r));

    vector<int> available(r);

    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            allocation[i][j] = gui_allocation[i][j];
            request[i][j] = gui_request[i][j];
        }
    }

    for (int j = 0; j < r; j++)
        available[j] = gui_available[j];

    DetectionResult result =
        detectDeadlock(allocation, request, available);

    *deadlockedCount = result.deadlockedProcesses.size();
    *completionCount = result.completionOrder.size();

    for (int i = 0; i < *deadlockedCount; i++)
        deadlockedProcesses[i] =
            result.deadlockedProcesses[i];

    for (int i = 0; i < *completionCount; i++)
        completionOrder[i] =
            result.completionOrder[i];

    return result.deadlockedProcesses.empty();
}
extern "C" int run_deadlock_recovery_from_gui(
    int p,
    int r,
    int allocation[20][20],
    int need[20][20],
    int available[20],
    int deadlockedProcess)
{
    if (deadlockedProcess < 0 || deadlockedProcess >= p)
        return 0;

    // Release all resources held by the deadlocked process
    for (int j = 0; j < r; j++)
    {
        available[j] += allocation[deadlockedProcess][j];
        allocation[deadlockedProcess][j] = 0;
        need[deadlockedProcess][j] = 0;
    }

    return 1;
}
extern "C" int run_rag_from_gui(
    int p,
    int r,
    int allocation[20][20],
    int request[20][20],
    int deadlockedProcesses[20],
    int deadlockedCount)
{
    vector<vector<int>> guiAllocation(
        p, vector<int>(r)
    );

    vector<vector<int>> guiRequest(
        p, vector<int>(r)
    );

    vector<int> deadlocked;

    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            guiAllocation[i][j] = allocation[i][j];
            guiRequest[i][j] = request[i][j];
        }
    }

    for (int i = 0; i < deadlockedCount; i++)
        deadlocked.push_back(deadlockedProcesses[i]);

    return writeRAG(
        guiAllocation,
        guiRequest,
        p,
        r,
        deadlocked
    ) ? 1 : 0;
}