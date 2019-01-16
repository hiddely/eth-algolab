//
// Created by Hidde Lycklama on 1/10/19.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <map>

typedef std::vector<std::vector<int> > DepMap;

struct DefaultMinusOne {
    int val = -1;

    DefaultMinusOne() = default;
};

std::map<std::pair<int, std::vector<bool> >, DefaultMinusOne> DP;

long calc(int i, std::vector<bool> selected, int len, std::vector<int> &costs, std::vector<int> &rewards, DepMap &deps) {
    if (i == len) {
        return 0;
    }
    if (DP[std::pair<int, std::vector<bool> >(i, selected)].val != -1) {
//        std::cerr << i << " returning " << DP[std::pair<int, std::vector<bool> >(i, selected)].val <<std::endl;
        return DP[std::pair<int, std::vector<bool> >(i, selected)].val;
    }
    std::vector<bool> oldSelected = selected;
    // dont pick
    long o1 = calc(i + 1, selected, len, costs, rewards, deps);
    // pick
    long cost = 0;
    for (int j = 0; j < deps[i].size(); j++) {
        int zone = deps[i][j];
        if (!selected[zone]) {
            // add cost
            cost += costs[zone];
            selected[zone] = true;
        }
    }
    long o2 = -cost + rewards[i] + calc(i + 1, selected, len, costs, rewards, deps);
    long v = std::max(o1, o2);
    DP[std::pair<int, std::vector<bool> >(i, oldSelected)].val = v;
    return v;
}

void testcase() {
    int numZones, numJobs;
    std::cin >> numZones >> numJobs;

    DP = std::map<std::pair<int, std::vector<bool> >, DefaultMinusOne>();

    std::vector<int> costs(numZones);
    std::vector<int> rewards(numJobs);
    for (int i = 0; i < numZones; i++) {
        int c; std::cin >> c;
        costs[i] = c;
    }
    for (int i = 0; i < numJobs; i++) {
        int r; std::cin >> r;
        rewards[i] = r;
    }

    DepMap d(numJobs);
    for (int i = 0; i < numJobs; i++) {
        int n; std::cin >> n;
        d[i] = std::vector<int>(n);
        for (int j = 0; j < n; j++) {
            int z; std::cin >> z;
            d[i][j] = z;
        }
    }
    std::vector<bool> selected(numZones);

    long c = calc(0, selected, numJobs, costs, rewards, d);
    std::cout << c << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}