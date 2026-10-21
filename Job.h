
#pragma once

#include <string>
#include <vector>

struct Job {
    std::string id;
    int deadline;
    int profit;
};

struct ScheduledJob {
    int slot;
    Job job;
};

struct ScheduleResult {
    std::vector<ScheduledJob> schedule;
    int totalProfit = 0;
    double timeMs = 0.0;
};
