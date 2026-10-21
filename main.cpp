
#include "Job.h"
#include "Scheduler.h"
#include "DP.h"

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <chrono>

using namespace std;

int main() {
    int n;

    cout << "====================================\n";
    cout << "       TASK SCHEDULER SYSTEM\n";
    cout << "====================================\n";

    cout << "Enter number of jobs (1-500): ";

    if (!(cin >> n) || n < 1 || n > 500) {
        cout << "Invalid number of jobs.\n";
        return 1;
    }

    vector<Job> jobs;
    jobs.reserve(n);

    cout << "\nEnter details for each job:\n";
    cout << "ID must be unique; deadline >= 1; profit >= 0.\n\n";

    for (int i = 0; i < n; i++) {
        Job job;

        cout << "Job " << i + 1 << " ID: ";
        if (!(cin >> job.id)) {
            cout << "Invalid job ID.\n";
            return 1;
        }

        for (const Job& existing : jobs) {
            if (existing.id == job.id) {
                cout << "Duplicate job ID. Restart and use unique IDs.\n";
                return 1;
            }
        }

        cout << "Deadline: ";
        if (!(cin >> job.deadline) || job.deadline < 1) {
            cout << "Deadline must be at least 1.\n";
            return 1;
        }

        cout << "Profit: ";
        if (!(cin >> job.profit) || job.profit < 0) {
            cout << "Profit cannot be negative.\n";
            return 1;
        }

        jobs.push_back(job);
        cout << "\n";
    }

    cout << "========== GREEDY MAX-HEAP ==========\n";

    ScheduleResult result = scheduleWithMaxHeap(jobs);

    cout << "\nScheduled Jobs:\n";
    cout << "Slot\tJob ID\tDeadline\tProfit\n";

    for (const auto& entry : result.schedule) {
        cout << entry.slot << '\t'
             << entry.job.id << '\t'
             << entry.job.deadline << '\t'
             << entry.job.profit << '\n';
    }

    cout << "\nTotal Profit: " << result.totalProfit << '\n';
    cout << "Greedy Execution Time: "
         << result.timeMs << " ms\n";

    cout << "Schedule Valid: "
         << (validateSchedule(result) ? "YES" : "NO")
         << '\n';

    cout << "\n====== DYNAMIC PROGRAMMING ======\n";

    auto start = chrono::steady_clock::now();
    long long dpProfit = maxProfitDP(jobs);
    auto finish = chrono::steady_clock::now();

    double dpTimeMs =
        chrono::duration<double, milli>(finish - start).count();

    cout << "Maximum Profit: " << dpProfit << '\n';
    cout << "DP Execution Time: " << dpTimeMs << " ms\n";

    cout << "\n========== COMPARISON ==========\n";
    cout << "Greedy Profit: " << result.totalProfit << '\n';
    cout << "DP Profit:     " << dpProfit << '\n';

    if (result.totalProfit == dpProfit) {
        cout << "Result: Both algorithms agree.\n";
    } else {
        cout << "Result: Profits differ. Check the implementations.\n";
    }

    cout << "\nProgram completed successfully.\n";

    return 0;
}
