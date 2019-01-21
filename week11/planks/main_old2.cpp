//
// Created by Hidde Lycklama on 12/22/18.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <numeric>
#include <tuple>
#include <map>

typedef std::tuple<int, int, int, int> Square;

struct DefaultMin {
    int val = -1;
};

void testcase() {
    int n; std::cin >> n;
    int totalCount = 0;
    std::vector<int> planks(n);
    for (int i = 0; i < n; i++) {
        std::cin >> planks[i];
        totalCount += planks[i];
    }

    const int sumSide = totalCount / 4;

    if (totalCount % 4 != 0) {
        std::cout << 0 << std::endl;
        return;
    }

    int firstHalf = n / 2;
    int secondHalf = n - firstHalf;

    unsigned int options = 0;

    std::vector<Square> partials(1 << (2 * firstHalf));
    for (int s = 0; s < 1 << (2 * firstHalf); s++) {
        int s1 = 0, s2 = 0, s3 = 0, s4 = 0;
        for (int i = 0 ; i < firstHalf; i++) {
            if (!(s & (1 << (2 * i))) && !(s & (1 << ((2 * i) + 1)))) {
                s1 += planks[i];
            } else if (s & (1 << (2 * i)) && !(s & (1 << ((2 * i) + 1)))) {
                s2 += planks[i];
            } else if (!(s & (1 << (2 * i))) && (s & (1 << ((2 * i) + 1)))) {
                s3 += planks[i];
            } else if ((s & (1 << (2 * i))) && (s & (1 << ((2 * i) + 1)))) {
                s4 += planks[i];
            }
        }
//        if (s1 > sumSide || s2 > sumSide || s3 > sumSide || s4 > sumSide) {
//            continue;
//        }
        partials[s] = Square(sumSide - s1, sumSide - s2, sumSide - s3, sumSide - s4);
        // potential early termination
    }

    std::sort(partials.begin(), partials.end());

//    std::vector<Square> second(1 << (2 * secondHalf));
//    std::map<Square, DefaultMin> cache;
    for (int s = 0; s < 1 << (2 * secondHalf); s++) {
        int s1 = 0, s2 = 0, s3 = 0, s4 = 0;
        for (int i = 0 ; i < secondHalf; i++) {
            if (!(s & (1 << (2 * i))) && !(s & (1 << ((2 * i) + 1)))) {
                s1 += planks[firstHalf + i];
            } else if (s & (1 << (2 * i)) && !(s & (1 << ((2 * i) + 1)))) {
                s2 += planks[firstHalf + i];
            } else if (!(s & (1 << (2 * i))) && (s & (1 << ((2 * i) + 1)))) {
                s3 += planks[firstHalf + i];
            } else if ((s & (1 << (2 * i))) && (s & (1 << ((2 * i) + 1)))) {
                s4 += planks[firstHalf + i];
            }
        }
//        if (s1 > sumSide || s2 > sumSide || s3 > sumSide || s4 > sumSide) {
//            continue;
//        }

        Square res = Square(s1, s2, s3, s4);
        auto bounds = std::equal_range(partials.begin(), partials.end(), res);
        options += std::distance(bounds.first, bounds.second);
//        if (cache[res].val != -1) {
//            options += cache[res].val;
//        } else {
//            auto iter = std::lower_bound(partials.begin(), partials.end(), res);
//            int count = 0;
//            while (iter != partials.end() && *iter == res) {
//                options++;
//                count++;
//                iter++;
//            }
//            cache[res].val = count;
//        }

//        for (auto iter = partials.begin(); iter != partials.end(); iter++) {
////            std::cerr << "Testing " << std::get<0>(*iter) << " " << std::get<1>(*iter) << " " << std::get<2>(*iter) << " " << std::get<3>(*iter) << std::endl;
//            if (s1 + std::get<0>(*iter) == sumSide
//                && s2 + std::get<1>(*iter) == sumSide
//                    && s3 + std::get<2>(*iter) == sumSide
//                        && s4 + std::get<3>(*iter) == sumSide) {
//            }
//        }
        // potential early termination
    }

    std::cout << options / 24 << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}