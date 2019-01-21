//
// Created by Hidde Lycklama on 11/23/18.
//

#include <iostream>
#include <vector>

int DP(int i, std::vector<bool> bitmask, int w, int h, std::vector<std::vector<bool > > &input) {
    if (i == h) {
        return 0;
    }

    // we can pick or not
    for (int s = 0; s < 1 << w; s++) {
        std::vector<bool> bit(w);
        for (int i = 0; i < w; i++) {
            if (s & (1<<i) && input[i][w]) {
                // use it
                bit[i] = true;
            }
        }
        //  ???
        DP
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