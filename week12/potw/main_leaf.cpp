//
// Created by Hidde Lycklama on 12/3/18.
//

#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
// BGL includes
void testcase(int t) {
    int n, m, k;
    std::cin >> n >> m >> k;

    std::vector<int> temps(n);
    for (int i = 0; i < n; i ++) {
        std::cin >> temps[i];
    }

    std::vector<int> predmap(n);
    std::vector<bool> leafs(n, true);
    predmap[0] = -1;
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        predmap[v] = u;
        leafs[u] = false;
    }

//    std::cerr << "Start main loop" << std::endl;


    std::vector<int> starts;
    starts.reserve(n);
    std::vector<bool> reached(n, false); // optimization for tree structure
    for (int i = 0; i < n; i++) {
        if (leafs[i]) {
            int current = i;
            int low = i;
            int windowSize = 0;
//            std::cerr << "Starting from " << i << std::endl;
            std::multiset<int> window;
            int reachedCounter = 0;
            while (current != -1) {
                if (reachedCounter >= m - 1) {
                    break; // were already here
                }
                if (reached[current] ) {
                    reachedCounter++;
                }
                // traverse to top
                reached[current] = true;
//                std::cerr << current << ": Inserting " << temps[current] << std::endl;
                window.insert(temps[current]);
                if (windowSize <= m) {
                    windowSize++;

                    if (windowSize < m) {
//                    std::cerr << "Windowsize not on par " << windowSize << " " << m << std::endl;
                        current = predmap[current];
                        continue;
                    }
                }

                if (windowSize > m) {
                    // remove last
//                    std::cerr << "Removing " << temps[low] << std::endl;
//                    windowSize--;
                    window.erase(window.lower_bound(temps[low]));
                    low = predmap[low]; // increment lower bound
                }
//                std::cerr << current << ": " << min << " " << max << std::endl;
                // else, see if match
                if (*window.rbegin() - *window.begin() <= k) {
//                    std::cerr << max << " - " << min << " is alright. Pushing " << current << ", from: " << current << std::endl;
                    starts.push_back(current);
                }
                current = predmap[current];
            }

        }
    }

    std::sort(starts.begin(), starts.end());
    if (starts.empty()) {
        std::cout << "Abort mission" << std::endl;
    } else {
        int last = -1;
        for (auto iter = starts.cbegin(); iter != starts.cend(); iter++) {
            if (last == *iter) {
                continue;
            }
            last = *iter;
            std::cout << *iter << " ";
        }
        if (t > 1) {
            std::cout << "\n";
        } else {
            std::cout << std::endl;
        }
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase(t);
    }
    return 0;
}