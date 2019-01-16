//
// Created by Hidde Lycklama on 1/10/19.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <map>

typedef std::vector<std::vector<int> > DepMap;

void testcase() {
    int numZones, numJobs;
    std::cin >> numZones >> numJobs;

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
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}