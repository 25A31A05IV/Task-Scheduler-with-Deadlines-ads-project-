
#include "Scheduler.h"
#include "Job.h"

#include <algorithm>
#include <chrono>
#include <queue>
#include <set>

using namespace std;

struct ByProfit {
    bool operator()(const Job& a, const Job& b) const {
        if (a.profit != b.profit)
            return a.profit < b.profit;

        if (a.deadline != b.deadline)
            return a.deadline > b.deadline;

        return a.id > b.id;
    }
};

ScheduleResult scheduleWithMaxHeap(vector<Job> jobs) {
    auto start = chrono::steady_clock::now();
    ScheduleResult result;

    jobs.erase(remove_if(jobs.begin(), jobs.end(),
        [](const Job& j) {
            return j.deadline < 1 || j.profit < 0;
        }), jobs.end());

    if (jobs.empty()) {
        result.timeMs =
            chrono::duration<double, milli>(
                chrono::steady_clock::now() - start
            ).count();
        return result;
    }

    int n = jobs.size();
    int maxDeadline = 0;

    for (const Job& j : jobs)
        maxDeadline = max(maxDeadline, j.deadline);

    int lastSlot = min(maxDeadline, n);

    sort(jobs.begin(), jobs.end(),
        [](const Job& a, const Job& b) {
            if (a.deadline != b.deadline)
                return a.deadline > b.deadline;
            if (a.profit != b.profit)
                return a.profit > b.profit;
            return a.id < b.id;
        });

    priority_queue<Job, vector<Job>, ByProfit> pq;
    vector<ScheduledJob> chosen;
    int i = 0;

    for (int slot = lastSlot; slot >= 1; --slot) {
        while (i < n && jobs[i].deadline >= slot) {
            pq.push(jobs[i]);
            ++i;
        }

        if (!pq.empty()) {
            Job best = pq.top();
            pq.pop();

            chosen.push_back({slot, best});
            result.totalProfit += best.profit;
        }
    }

    reverse(chosen.begin(), chosen.end());
    result.schedule = chosen;

    result.timeMs =
        chrono::duration<double, milli>(
            chrono::steady_clock::now() - start
        ).count();

    return result;
}

bool validateSchedule(const ScheduleResult& result) {
    set<int> usedSlots;
    set<string> usedJobs;
    long long sum = 0;

    for (const auto& entry : result.schedule) {
        if (entry.slot < 1 ||
            entry.slot > entry.job.deadline)
            return false;

        if (!usedSlots.insert(entry.slot).second)
            return false;

        if (!usedJobs.insert(entry.job.id).second)
            return false;

        sum += entry.job.profit;
    }

    return sum == result.totalProfit;
}
