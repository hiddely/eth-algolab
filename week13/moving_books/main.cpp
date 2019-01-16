//
// Created by Hidde Lycklama on 1/16/19.
//

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
#include <cmath>
#include <climits>

void testcase() {
    int n, m;
    std::cin >> n >> m;

    std::map<int, int> personCount;
    std::vector<int> strs(n);
    std::vector<int> boxes(m);
    for (int i = 0; i < n; i++) {
        std::cin >> strs[i];
        personCount[strs[i]]++;
    }
    for (int i = 0; i < m; i++) {
        int b; std::cin >> b;
        boxes[i] = b;
    }

    std::sort(strs.begin(), strs.end(), std::greater<int>());
    std::sort(boxes.begin(), boxes.end(), std::greater<int>());

    if (strs[0] < *boxes.begin()) {
        // there is an unmovable box
        std::cout << "impossible" << std::endl;
        return;
    }

    int max = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (i == n - 1 || strs[i + 1] != strs[i]) {
            // at the last item OR at a boundary (for doubles)
            // find the items until now
            int boxCount = 0;
            if (i == n - 1) {
                boxCount = m;
            } else {
                for (int x = 0; x < m; x++) { // INEFFICIENT
                    if (boxes[x] > strs[i + 1]) {
                        boxCount++;
                    }
                }
            }
//            std::cerr << boxCount << " at " << i << " " << strs[i] << " " <<  (int)ceil((double)boxCount / (double)(i + 1)) << std::endl;

            // smear the boxCount over the amount of items !!!
            max = std::max(max, (int)ceil((double)boxCount / (double)(i + 1)));
        }
    }
    std::cout << ((max - 1) * 3) + 2 << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}