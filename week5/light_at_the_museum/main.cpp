#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <climits>

typedef std::vector<std::vector<std::pair<int, int> > > Table;

typedef std::pair<std::vector<int>, int > Result;

void testcase() {
    int N, M; // Num Switches, num rooms
    std::cin >> N >> M;

    std::vector<int> targets(M); // M is per room
    for (int i = 0; i < M; i++) {
        int b; std::cin >> b;
        targets[i] = b;
    }

    // Lights per room
    Table switches(N, std::vector<std::pair<int ,int> >(M));
    for (int a = 0; a < N; a++) {
        for (int b = 0; b < M; b++) {
            int on, off; std::cin >> on >> off;
            switches[a][b] = std::pair<int, int>(on, off);
        }
    }

    int firstHalf = N / 2;
    int secondHalf = N - firstHalf;

    std::vector<Result > halfs;

    for (int s = 0; s < 1 << firstHalf; s++) {
        int numFlips = __builtin_popcount(s);
        std::vector<int> lights = targets;
        for (int i = 0; i < firstHalf; i++) {
            for (int x = 0; x < M; x++ ) {
                if (s & (1 << i)) {
                    lights[x] -= switches[i][x].second;
                } else {
                    lights[x] -= switches[i][x].first;
                }
            }
        }
        halfs.push_back(Result(lights, numFlips));
    }

    std::sort(halfs.begin(), halfs.end());

    int min = INT_MAX;
    for (int s = 0; s < 1 << secondHalf; s++) {
        int numFlips = __builtin_popcount(s);
        std::vector<int> lights(M, 0);
        for (int i = 0; i < secondHalf; i++) {
            for (int x = 0; x < M; x++ ) {
                if (s & (1 << i)) {
                    lights[x] += switches[firstHalf + i][x].second;
                } else {
                    lights[x] += switches[firstHalf + i][x].first;
                }
            }
        }

        auto lower = std::lower_bound(halfs.begin(), halfs.end(), Result(lights, 0));
        while (lower != halfs.end() && lower->first == lights) {
            if (lower->second + numFlips < min) {
                min = lower->second + numFlips;
            }
            lower++;
        }
    }

    if (min == INT_MAX) {
        std::cout << "impossible" << std::endl;
    } else {
        std::cout << min << std::endl;
    }

    // first we calculate the subset?

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}