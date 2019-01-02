//
// Created by Hidde Lycklama on 12/22/18.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <numeric>

struct Less {
    bool operator() (int i,int j) { return (i > j);}
} Less;

int totalCount = 0;
std::vector<int> combinations;

std::vector<int> firstCombs;
std::vector<int> secondCombs;

void combination(int offset, int k, int n, std::vector<int>::iterator items, int itemSize, int accumulation) {
    if (k == 0) {
        // check if we have a match

//        bool t1 = (combinations[0] & combinations[1]) == 0;
//        bool t2 = (combinations[1] & combinations[2]) == 0;
//        bool t3 = (combinations[2] & combinations[3]) == 0;
//        bool t4 = (combinations[0] & combinations[2]) == 0;
//        bool t5 = (combinations[1] & combinations[3]) == 0;
//        bool t6 = (combinations[0] & combinations[3]) == 0;
//
////        int test = combinations[0] + combinations[1] + combinations[2] + combinations[3];
////        std::cout << combinations[0] << " " << combinations[1] << " " << combinations[2] << " " << combinations[3] << std::endl;
//        if (t1 && t2 && t3 && t4 && t5 && t6) {
//            std::cout << k << " " << test << " " << max << std::endl;
            totalCount++;
//        }
        return;
    }
    for (int i = offset; i <= itemSize - k; ++i){
        if ((accumulation & *(items + i)) == 0) {
            // we can add
//            combinations.push_back(*(items + i));
            accumulation |= *(items + i);
//        std::cout << "Adding " << items[i] << std::endl;
            combination(i+1, k-1, n, items, itemSize, accumulation);
            accumulation ^= *(items + i);
//            combinations.pop_back();
        }

    }
}

void testcase() {
    int n; std::cin >> n;
    totalCount = 0;
    combinations = std::vector<int>();
    firstCombs = std::vector<int>();
    secondCombs = std::vector<int>();
    std::vector<int> planks(n);
    for (int i = 0; i < n; i++) {
        std::cin >> planks[i];
    }

//    std::sort(planks.begin(), planks.end(), Less);

    long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += planks[i];
    }
    const long expected = sum / 4;

    const int permCount = 1 << n;
//    std::cout << "N " << n << " " << permCount << std::endl;
    bool found = false;
    std::vector<int> numPerms;
    for (int i = 0; i < permCount; i++) {
//        std::list<int> s1,s2,s3,s4;
        long sum = 0;
        for (int o = 0; o < n; o++) {
            const long shifted = i >> 1 * o;
            const int masked = shifted & 1; // last two
//            std::cerr << "Masked " << masked << std::endl;
            if (masked == 0) {
                // do nothing
            } else if (masked == 1) {
                sum += planks[o];
            } else {
                std::cerr << "An error occurred: " << masked << std::endl;
                abort();
            }
        }
        if (sum == expected) {
//            std::cout << "Comb: " << std::bitset<32>(i) << std::endl;
            numPerms.push_back(i);
        }
    }

    // now find how many possible combinations there are,
    // O( 4^numPerms )
    // no sth else
    const int numPermsSize = numPerms.size();
//    int total = 1 << n;
//std::cout << numPermsSize <<  " AA " << std::endl;

    for (auto iter = numPerms.begin(); iter != numPerms.end(); iter++) {
        std::cout << std::bitset<20>(*iter) << std::endl;
    }

//    std::vector<int> combinations(4);
    if (numPermsSize >= 4) {
//        const int firstHalf = numPermsSize / 2;
//        const int secondHalf = numPermsSize - firstHalf;
        combination(0, 4, n, numPerms.begin(), numPermsSize, 0);
//        combination(0, 2, n,)
    }

    std::cout << totalCount << std::endl;

//    long curSum = 0;
//    const long lim = sum / 4;
//    std::cerr << "Lim " << lim << std::endl;
//    int atSide = 0;
//    std::vector<bool> visited(n, false);
//    for (int i = 0; i < n; i++) {
//        if (visited[i]) {
//            continue;
//        }
//        if (curSum + planks[i] <= lim) {
//            curSum += planks[i];
//            std::cerr << "Building side " << atSide << " with " << planks[i] << std::endl;
//            visited[i] = true;
//            if (curSum == lim) {
//                curSum = 0;
//                i = 0;
//                atSide++;
//            }
//        }
//    }

//    if (found) {
//        std::cout << "1" << std::endl;
//    }else {
//        std::cout << "0" << std::endl;
//    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}