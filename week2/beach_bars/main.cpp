//
// Created by Hidde Lycklama on 9/24/18.
//

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

void testcase() {
    int base = 1000000;
    int maxIndex = 2 * base;
    int n; std::cin >> n;
    std::vector<bool> parasols(maxIndex, false);

    // We try another sliding window
    int maxDynamicIndex = std::numeric_limits<int>::min();
    int minDynamicIndex = std::numeric_limits<int>::max();
    for (int i = 0; i < n; i++) {
        int index; std::cin >> index;
        parasols[index + base] = true;
        if (index + base > maxDynamicIndex) {
            maxDynamicIndex = index + base + 100;
        }
        if (index + base < minDynamicIndex) {
            minDynamicIndex = index + base ;
        }
    }

    minDynamicIndex = std::max(minDynamicIndex - 200, 0);
    maxDynamicIndex = std::max(maxDynamicIndex, minDynamicIndex + 200);

    int sum = parasols[minDynamicIndex];
    int a = minDynamicIndex; int b = minDynamicIndex;
    int maxNum = sum;
    int sliding = 200;
    std::set<int> coordinates;
    while (b <= maxDynamicIndex) {
        if (sliding == 0) {
            // Fill up first
            sum -= parasols[a];
            a++;
        } else {
            sliding--;
        }
        b++;
        sum += parasols[b];

        if (sum > 0) {
//            std::cout << a << " " << sum << std::endl;
        }

        if (sum > maxNum) {
            coordinates.clear();
//            std::cout << a - base << " " << sum << std::endl;
            coordinates.insert(a + 100 + (sliding / 2));
            maxNum = sum;
        } else if (sum == maxNum) {
            coordinates.insert(a + 100 + (sliding / 2));
        }
    }

    std::cout << maxNum << " ";

    // calculate best coordinates
    std::vector<std::set<int>> minDistances(100);

    auto it = coordinates.begin();
    for (it = coordinates.begin(); it != coordinates.end(); ++it)
    {
        int distance = -1;
        for (int i = 100; i > 0; i--) {
            if (*it - i >= 0) {
                bool left = parasols[*it - i];
                if (left) {
                    distance = i;
//                    std::cout << "Min Dist " << *it - base << " " << i << std::endl;
                    break;
                }
            }
            if (*it + i < maxIndex) {
                bool right = parasols[*it + i];
                if (right) {
                    distance = i;
//                    std::cout << "Min Dist " << i << std::endl;
                    break;
                }
            }
        }
        minDistances[distance-1].insert(*it);
    }
    for (int i = 0; i < 100; i++) {
        if (!minDistances[i].empty()) {
            std::cout << i + 1 << '\n';
            auto dit = minDistances[i].begin();
            bool first = true;
            for (dit = minDistances[i].begin(); dit != minDistances[i].end(); ++dit)
            {
                if (first) {
                    first = false;
                } else {
                    std::cout << " ";
                }
                std::cout << *dit - base;
            }
            break;
        }
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
