//
// Created by Hidde Lycklama on 1/6/19.
//

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

const bool VERBOSE = false;

struct DefaultMinusOne {
    int val = -1;
};

//int combiFour[4][4] = {
//        {
//                1, 1, 1, 1
//        },
//        {
//                1, 1, 1, 0
//        },
//        {
//                1, 1, 0, 1
//        },
//        {
//                1, 0, 1, 1
//        },
//        {
//                0, 1 ,1, 1
//        },
//        {
//                1, 1, 0, 0
//        },
//        {
//                1, 0, 0, 1
//        },
//        {
//                0, 0, 1, 1
//        },
//        {
//                1, 0, 1, 0
//        },
//        {
//                1, 1, 0, 1
//        },
//};
//
//int combiThree[4][3] = {
//        {
//            1, 1, 1
//        },
//        {
//            0, 1, 1
//        },
//        {
//            1, 1, 0
//        },
//        {
//            1, 0, 1
//        }
//};
//
//int combiTwo[1][2] = {
//        {
//                1, 1
//        },
//};

std::map<std::vector<int>, DefaultMinusOne> DP;

//int[][] chooseX(int i, int count, std::vector<int> choices, int num, std::vector<int> &indexes) {
//    if (count == num) {
//        std::vector<int> v = std::vector<int>();
//        return choices;
//    }
//    for (int a = i; a < n; a++) {
//        std::vector<int> ayy = choices;
//        ayy.push_back(indexes[a]);
//        chooseX(a + 1, count, ayy, num, indexes);
//    }
//}

int calc(std::vector<int> pointers, int n, std::vector<std::vector<int>> &stacks) {
    if (DP[pointers].val != -1) {
        return DP[pointers].val;
    }
    bool allZero = true;
    int sum = 0;
    std::map<int, int> counts;
    if (VERBOSE) {

        for (int i = 0; i < n; i++) { // basic decrement
            if (pointers[i] >= 0) {
                if (VERBOSE) std::cerr << stacks[i][pointers[i]] << " ";
            } else {
                if (VERBOSE) std::cerr << "end ";
            }
        }
        if (VERBOSE) std::cerr << std::endl;
    }
    for (int i = 0; i < n; i++) { // basic decrement
        if (pointers[i] != -1) {
            allZero = false;
            counts[stacks[i][pointers[i]]]++;

            std::vector<int> cp = pointers;
            cp[i]--;
            sum = std::max(sum, calc(cp, n, stacks));
        }
    }
    if (allZero) {
        return 0; // base case
    }
    // check number of items equal
    for (auto iter = counts.begin(); iter != counts.end(); iter++) {
        if (iter->second > 1) {

            std::vector<int> indexes;
            for (int i = 0; i < n; i++) {
                if (pointers[i] != -1 && stacks[i][pointers[i]] == iter->first) {
                    indexes.push_back(i);
                }
            }
            // out of indexes, choose
            int nums = 1 << iter->second;
            for (int i = 0; i < nums; i++) {
                int numChosen = 0;
                std::vector<int> cp = pointers;
                for (int a = 0; a < iter->second; a++) {
                    if (i & (1 << a)) {
                        cp[indexes[a]]--;
                        numChosen++;
                    }
                }
                if (numChosen >= 2) {
                    // at least 2
                    sum = std::max(sum, ((int) pow(2, numChosen - 2)) + calc(cp, n, stacks));
                }
            }
////            for (int i = iter->second; i > 1; i++) {
////                // Remove i
////
////            }
//
//            std::vector<int> cp = pointers;
//            for (int i = 0; i < n; i++) {
//                if (cp[i] == -1) {
//                    continue;
//                }
//                if (stacks[i][cp[i]] == iter->first) {
//                    cp[i]--;
//                }
//            }
////            int x = std::
//            int v;
//            if (iter->second - 2 == 0) {
//                v = 1;
//            } else {
//                v = ((int)pow(2, iter->second - 2));
//            }
////            if ((pointers[2] == 9 && pointers[4] == 9)) {
////                std::cerr << "Found " << iter->first << " " << iter->second << " times! ";
////                std::cerr << v << " + " << calc(cp, n, stacks) << std::endl;
////            }
//            sum = std::max(sum, v + calc(cp, n, stacks));
        }
    }
    DP[pointers].val = sum;
//    if ((pointers[2] == 10 && pointers[4] == 10) || (pointers[2] == 9 && pointers[4] == 9)) {
//        for (int i = 0; i < n; i++) { // basic decrement
//            if (pointers[i] != -1) {
//                std::cerr << stacks[i][pointers[i]] << " (i: " << pointers[i] << ") ";
//            } else {
//                std::cerr << "end ";
//            }
//        }
//        std::cerr << " S " << sum << std::endl;
//    }
    return sum;
}

void testcase() {
    int n; std::cin >> n;
    std::vector<int> sizes(n);
    for (int i = 0; i < n; i++) {
        std::cin >> sizes[i];
    }
    DP = std::map<std::vector<int>, DefaultMinusOne>();
    std::vector<std::vector<int>> stacks(n);
    for (int i = 0; i < n; i++) {
        stacks[i] = std::vector<int>(sizes[i]);
        for (int a = 0; a < sizes[i]; a++) {
            std::cin >> stacks[i][a];
        }
        sizes[i]--; // init to pointer arrach
    }

    int score = calc(sizes, n, stacks);

    std::cout << score << std::endl;

    if (VERBOSE) {
        for (auto iter = DP.begin(); iter != DP.end(); iter++) {
            if (iter->first[2] != 16) {
                continue;
            }
            for (int i = 0; i < n; i++) {
                std::cerr << iter->first[i] << " ";
            }
            std::cerr << ": " << iter->second.val << std::endl;
//            std::cerr << iter->first <<
        }
    }
//    std::vector<int> k = std::vector<int>(n);
//    k[0] = 2;
//    k[1] = 4;
//    k[2] = 10;
//    k[3] = 2;
//    k[4] = 10;
//    std::cerr << DP[k].val << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}