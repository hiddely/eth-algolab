//
// Created by Hidde Lycklama on 11/23/18.
//

#include <iostream>
#include <vector>

int DP(int i, std::vector<bool> bitmask, int &w, std::vector<std::vector<bool > > &input) {
    if (i == 0) {
        return 0;
    }
    std::vector<bool> emptyMask(w)
    if (bitmask == emptyMask) {
        return DP(i - 1, std::vector<bool>(w, true));
    }

}

void testcase() {
    int h, w;
    std::cin >> h >> w;

    std::vector<std::vector<bool > > input(h, std::vector<bool>(w));
    for (int a = 0; a < h; a++) {
        for (int b = 0; b < w; b++) {
            std::cin >> input[a][b];
        }
    }
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}