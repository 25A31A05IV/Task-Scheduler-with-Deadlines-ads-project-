#pragma once

#include "Job.h"
#include <vector>

ScheduleResult scheduleWithMaxHeap(std::vector<Job> jobs);

bool validateSchedule(const ScheduleResult& result);