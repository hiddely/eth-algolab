//
// Created by Hidde Lycklama on 1/9/19.
//

#include <iostream>
#include <vector>

void calc(int ind, int numDisks, int m, std::vector<int> &disks, std::vector<bool> &DP) {
    if (ind == numDisks) {
        return;
    }
    std::vector<bool> temp(m);
    for (int i = 0; i < m; i++) {
        if (DP[i]) {
//            std::cerr << "DP Setting " << i << " + " << disks[ind] << " to true. " <<std::endl;
            temp[(i + disks[ind]) % m] = true;
        }
    }
    for (int i = 0; i < m; i++) {
        if (temp[i]) {
            DP[i] = true;
        }
    }
//        std::cerr << "Setting " << disks[ind] << " to true. " <<std::endl;
    DP[disks[ind]] = true;
    calc(ind + 1, numDisks, m, disks, DP);
}

void testcase() {
    int n, i, k;
    std::cin >> n >> i >> k;

    std::vector<int> disks(n);
    for (int i = 0; i < n; i++) {
        int d; std::cin >> d;
        disks[i] = d % k;
    }

    std::vector<bool> DP(k);
    calc(0, n, k, disks, DP);

    if (DP[i]) {
        std::cout << "yes" << std::endl;
    } else {
        std::cout << "no" << std::endl;
    }

//    for (int i = 0; i < n; i++) {
//        std::cerr << disks[i] << std::endl;
//
//    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0 ) {
        testcase();
    }
    return 0;
}