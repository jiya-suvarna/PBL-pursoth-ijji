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

int main() {
    int n, m;

    cout << "========================================\n";
    cout << " STUDENT 4: DEADLOCK DETECTION AND RAG\n";
    cout << "========================================\n";

    cout << "\nEnter number of processes: ";
    if (!(cin >> n) || n <= 0 || n > 100) {
        cout << "Invalid number of processes.\n";
        return 1;
    }

    cout << "Enter number of resource types: ";
    if (!(cin >> m) || m <= 0 || m > 100) {
        cout << "Invalid number of resource types.\n";
        return 1;
    }

    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> request(n, vector<int>(m));
    vector<int> available(m);

    cout << "\nEnter ALLOCATION MATRIX:\n";

    for (int i = 0; i < n; i++) {
        cout << "P" << i + 1 << ": ";

        for (int j = 0; j < m; j++) {
            if (!(cin >> allocation[i][j]) ||
                allocation[i][j] < 0) {
                cout << "Invalid allocation value.\n";
                return 1;
            }
        }
    }

    cout << "\nEnter CURRENT REQUEST MATRIX:\n";

    for (int i = 0; i < n; i++) {
        cout << "P" << i + 1 << ": ";

        for (int j = 0; j < m; j++) {
            if (!(cin >> request[i][j]) ||
                request[i][j] < 0) {
                cout << "Invalid request value.\n";
                return 1;
            }
        }
    }

    cout << "\nEnter AVAILABLE RESOURCE VECTOR: ";

    for (int j = 0; j < m; j++) {
        if (!(cin >> available[j]) ||
            available[j] < 0) {
            cout << "Invalid available resource value.\n";
            return 1;
        }
    }

    printMatrix(allocation, "ALLOCATION MATRIX", n, m);
    printMatrix(request, "CURRENT REQUEST MATRIX", n, m);
    printVector(available, "AVAILABLE VECTOR: ");

    DetectionResult result =
        detectDeadlock(allocation, request, available);

    cout << "\n========== DEADLOCK DETECTION ==========\n";

    if (result.deadlockedProcesses.empty()) {
        cout << "Result: No deadlock detected.\n";
        cout << "All processes can finish under the supplied requests.\n";
    } else {
        cout << "Result: DEADLOCK DETECTED!\n";
        cout << "Affected processes: ";

        for (int p : result.deadlockedProcesses)
            cout << "P" << p + 1 << " ";

        cout << '\n';
    }

    if (!result.completionOrder.empty()) {
        cout << "Processes that can finish: ";

        for (size_t i = 0; i < result.completionOrder.size(); i++) {
            if (i > 0)
                cout << " -> ";

            cout << "P" << result.completionOrder[i] + 1;
        }

        cout << '\n';
    }

    cout << "\n========== RESOURCE ALLOCATION GRAPH ==========\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {

            if (allocation[i][j] > 0) {
                cout << "R" << j + 1 << " -> P" << i + 1
                     << " (Allocated: " << allocation[i][j]
                     << ")\n";
            }

            if (request[i][j] > 0) {
                cout << "P" << i + 1 << " -> R" << j + 1
                     << " (Request: " << request[i][j]
                     << ")\n";
            }
        }
    }

    if (writeRAG(allocation, request, n, m,
                 result.deadlockedProcesses)) {
        cout << "\nResource Allocation Graph saved as rag.dot\n";
        cout << "Create graph image using:\n";
        cout << "dot -Tpng rag.dot -o rag.png\n";
    } else {
        cout << "Could not create the graph file.\n";
    }

    cout << "\nNote: Deadlock detection uses current requests.\n";
    cout << "Banker's Algorithm separately checks system safety.\n";

    return 0;
}