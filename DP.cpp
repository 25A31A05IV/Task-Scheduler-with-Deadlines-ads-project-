
#include "DP.h"

#include <algorithm>
#include <climits>

using namespace std;

long long maxProfitDP(vector<Job> jobs) {
    jobs.erase(remove_if(jobs.begin(), jobs.end(),
        [](const Job& j) {
            return j.deadline < 1 || j.profit < 0;
        }), jobs.end());

    sort(jobs.begin(), jobs.end(),
        [](const Job& a, const Job& b) {
            return a.deadline < b.deadline;
        });

    int n = jobs.size();
    const long long NEG = LLONG_MIN / 4;

    // dp[k] = maximum profit using exactly k jobs.
    vector<long long> dp(n + 1, NEG);
    dp[0] = 0;

    for (int i = 0; i < n; ++i) {
        int limit = min(i + 1, jobs[i].deadline);

        for (int k = limit; k >= 1; --k) {
            if (dp[k - 1] != NEG) {
                dp[k] = max(dp[k],
                            dp[k - 1] + jobs[i].profit);
            }
        }
    }

    return *max_element(dp.begin(), dp.end());
}
