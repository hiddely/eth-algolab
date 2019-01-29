#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <numeric>

typedef std::vector<bool> Line;
typedef std::vector<Line> Grid;

typedef std::vector<std::vector<int> > Table;

Table DP;

int Key(std::vector<bool> &in) {
    int o;
    o = std::accumulate(in.rbegin(), in.rend(), 0, [](int x, int y) { return (x << 1) + y; });
    return o;
}

int tiles(int i, Line mask, int w, Grid &G) {
    if (i < 0) {
        return 0; // end
    }
    int key = Key(mask);
    if (DP[i][key] != -1) {
        return DP[i][key];
    }
// check our mask for pairs of two
    int max = 0;
    std::vector<int> potentialPairs;
    for (int p = 0; p < w - 1; p++) {
        // we can take the pair or not
        if (mask[p] && mask[p + 1] && G[i][p] && G[i][p + 1]) {
            // Line nextMask = G[i];
            // nextMask[p] = false;
            // nextMask[p + 1] = false;
            // int s = 1 + tiles(i - 1, nextMask, w, G);
            // max = std::max(max, s);
            potentialPairs.push_back(p);
            // std::cerr << i << " Pair " << p << std::endl;
        }
    }
    for (int s = 0; s < 1 << potentialPairs.size(); s++) {
        Line nextMask = G[i];
        int num = 0;
        bool failed = false;
        // std::cerr << std::bitset<32>(s) << std::endl;
        for (int p = 0; p < potentialPairs.size(); p++) {

            if (s & (1 << p)) {
                if (!nextMask[potentialPairs[p]]) {
                    failed = true; // taken
                    break;
                }
                nextMask[potentialPairs[p]] = false;
                nextMask[potentialPairs[p] + 1] = false;
                // std::cerr << "C " << potentialPairs[p] << " ";
                num += 1;
            }
        }
        // std::cerr << std::endl;
        if (failed) {
            continue;
        }
        // std::cerr << " N " << num << std::endl;
        int val = num + tiles(i - 1, nextMask, w, G);
        max = std::max(max, val);
    }
    int noPair = tiles(i - 1, G[i], w, G);
    max = std::max(max, noPair);
    DP[i][key] = max;
    return max;
}

void testcase() {
    int h, w;
    std::cin >> h >> w;

    DP = Table(h, std::vector<int>(1 << w, -1));

    Grid g(h, Line(w));
    for (int a= 0; a < h; a++) {
        for (int b = 0; b < w; b++) {
            bool x; std::cin >> x;
            g[a][b] = x;
        }
    }

    int o = tiles(h - 1, Line(w, 0), w, g);
    std::cout << o << std::endl;
}

int main() {
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}