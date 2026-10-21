#include <algorithm>
#include <chrono>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "httplib.h"
#include "json.hpp"
#include "Job.h"
#include "Scheduler.h"
#include "DP.h"

using json = nlohmann::json;

int main() {
    httplib::Server server;

    server.Get("/api/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(R"({"status":"ok","message":"Task Scheduler C++ API is running"})", "application/json");
    });

    server.Post("/api/schedule", [](const httplib::Request& req, httplib::Response& res) {
        try {
            const json body = json::parse(req.body);
            if (!body.contains("jobs") || !body["jobs"].is_array()) {
                res.status = 400;
                res.set_content(R"({"error":"Request must contain a jobs array."})", "application/json");
                return;
            }

            const auto& inputJobs = body["jobs"];
            if (inputJobs.empty() || inputJobs.size() > 500) {
                res.status = 400;
                res.set_content(R"({"error":"Enter between 1 and 500 jobs."})", "application/json");
                return;
            }

            std::vector<Job> jobs;
            std::set<std::string> ids;
            for (const auto& item : inputJobs) {
                if (!item.is_object() || !item.contains("id") || !item.contains("deadline") || !item.contains("profit") ||
                    !item["id"].is_string() || !item["deadline"].is_number_integer() || !item["profit"].is_number_integer()) {
                    res.status = 400;
                    res.set_content(R"({"error":"Each job needs a string id and integer deadline/profit."})", "application/json");
                    return;
                }
                Job job{item["id"].get<std::string>(), item["deadline"].get<int>(), item["profit"].get<int>()};
                if (job.id.empty() || job.deadline < 1 || job.profit < 0) {
                    res.status = 400;
                    res.set_content(R"({"error":"Job IDs must be non-empty, deadlines at least 1, and profits non-negative."})", "application/json");
                    return;
                }
                if (!ids.insert(job.id).second) {
                    res.status = 400;
                    res.set_content((json{{"error", "Duplicate job ID: " + job.id}}).dump(), "application/json");
                    return;
                }
                jobs.push_back(job);
            }

            const auto greedyStart = std::chrono::steady_clock::now();
            ScheduleResult greedy = scheduleWithMaxHeap(jobs);
            const auto greedyEnd = std::chrono::steady_clock::now();
            double greedyTimeMs = std::chrono::duration<double, std::milli>(greedyEnd - greedyStart).count();

            const auto dpStart = std::chrono::steady_clock::now();
            long long dpProfit = maxProfitDP(jobs);
            const auto dpEnd = std::chrono::steady_clock::now();
            double dpTimeMs = std::chrono::duration<double, std::milli>(dpEnd - dpStart).count();

            json scheduled = json::array();
            for (const auto& entry : greedy.schedule) {
                scheduled.push_back({
                    {"slot", entry.slot},
                    {"id", entry.job.id},
                    {"deadline", entry.job.deadline},
                    {"profit", entry.job.profit}
                });
            }

            json output = {
                {"schedule", scheduled},
                {"totalProfit", greedy.totalProfit},
                {"timeMs", greedyTimeMs},
                {"dpProfit", dpProfit},
                {"dpTimeMs", dpTimeMs},
                {"valid", validateSchedule(greedy)},
                {"jobsSubmitted", jobs.size()},
                {"jobsScheduled", greedy.schedule.size()}
            };
            res.set_content(output.dump(), "application/json");
        } catch (const json::exception& e) {
            res.status = 400;
            res.set_content((json{{"error", std::string("Invalid JSON request: ") + e.what()}}).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content((json{{"error", std::string("Server error: ") + e.what()}}).dump(), "application/json");
        }
    });

    if (!server.set_mount_point("/", "./web")) {
        std::cerr << "Warning: could not mount ./web. Run server.exe from the project folder.\n";
    }

    std::cout << "Task Scheduler website running at http://127.0.0.1:8080\n";
    std::cout << "Press Ctrl+C to stop the server.\n";
    if (!server.listen("127.0.0.1", 8080)) {
        std::cerr << "Could not start server on port 8080. Is another app using it?\n";
        return 1;
    }
    return 0;
}
