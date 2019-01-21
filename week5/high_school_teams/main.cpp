//
// Created by Hidde Lycklama on 1/20/19.
//

#include <iostream>
#include <vector>
#include <algorithm>

//typedef std::pair<int, int> TeamRB;
typedef std::pair<int, int> Sum;
typedef std::vector<Sum> Res;

unsigned int countSetBits(unsigned int n)
{
    unsigned int count = 0;
    while (n)
    {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

void testcase() {
    int n, k;
    std::cin >> n >> k;

    std::vector<int> skills(n);
    for (int i = 0; i < n; i++) {
        std::cin >> skills[i];
    }

    const int firstHalf = n / 2;
    const int secondHalf = n - firstHalf;

    Res sumsOne;
    for (int x = 0; x < 1 << firstHalf; x++) {
        const int count = countSetBits(x);
        if (count > k) {
            continue;
        }

        std::vector<int> current;
        for (int i = 0; i < firstHalf; i++) {
            if (!(x & (1<<i))) {
                current.push_back(skills[i]);
            }
        }
        // First half
        for (int i = 0; i < 1 << (firstHalf - count); i++) {

            int rSum = 0;

//            std::cerr << std::bitset<32>(i) << std::endl;

            for (int a = 0; a < firstHalf - count; a++) {
                if (i & (1<<a)) {
                    rSum += current[a];
//                    std::cerr << "RA " << skills[a] << " ";
                } else {
//                    std::cerr << "BA " << skills[a] << " ";
                    rSum -= current[a];
                }
            }

//            std::cerr << i << " " << "Adding sum " << rSum << " " << bSum << " " << count << std::endl;
            sumsOne.push_back(Sum(rSum, count));
        }
    }

    std::sort(sumsOne.begin(), sumsOne.end());

    long options = 0;
    for (int x = 0; x < 1 << secondHalf; x++) {
        const int count = countSetBits(x);
        if (count > k) {
            continue;
        }

        std::vector<int> current;
        for (int i = 0; i < secondHalf; i++) {
            if (!(x & (1<<i))) {
                current.push_back(skills[firstHalf + i]);
            }
        }
        // First half
        for (int i = 0; i < 1 << (secondHalf - count); i++) {

            int bSum = 0;

            for (int a = 0; a < secondHalf - count; a++) {
                if (i & (1<<a)) {
                    bSum -= current[a];
                } else {
                    bSum += current[a];
                }
            }

            auto iter = std::lower_bound(sumsOne.begin(), sumsOne.end(), Sum(bSum, 0));
            while (iter != sumsOne.end() && iter->first == bSum) {
                if (iter->second + count <= k) {
//                    std::cerr << "Found ! " << iter->first << " " << bSum << ", " << iter->second << " " << count << std::endl;
                    options++;
                }
                iter++;
            }
        }

    }
    std::cout << options << std::endl;
}

int main() {
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}