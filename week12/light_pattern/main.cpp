//
// Created by Hidde Lycklama on 12/28/18.
//

#include <iostream>
#include <vector>

bool valForL(int i, int K, int X) {
    int a = i % K;
    // now find a'th bit of X;
//    std::cerr << std::bitset<32>(X) << " ";
//    std::cerr << std::bitset<32>(X >> a) << " ";
//    std::cerr << a << " " << i << " " << K << " ";
    int b = (int) (X >> a) & 1;
//    std::cerr << std::bitset<32>(b) << " ";
    return (bool)b;
}

void testcase() {

    int N, K, X;
    std::cin >> N >> K >> X;

    int totalCount = 0;
//    int numMovesRequired = 0;
    int oneCount = 0;
    std::vector<bool> bulbs(N);
    for (int i = 0; i < N; i++) {
        bool b; std::cin >> b;
        bulbs[i] = b;
    }
    int flipCount = 0;
    // Take the singles out
    for (int y = 1; y < N - 1; y++) {
        // slide window
        int x = y - 1; int z = y + 1;
        // flip singles
        if (bulbs[x] == bulbs[z] && bulbs[x] != bulbs[y]) {
            // flip bulby
            bulbs[y] = !bulbs[y];
            flipCount++;
//            std::cerr << "Manual flip " << y << std::endl;
        }
    }
    if (bulbs[0] != bulbs[1]) {
        bulbs[0] = !bulbs[0];
        flipCount++;
    }
    if (bulbs[N - 2] != bulbs[N - 1]) {
        bulbs[N - 1] = !bulbs[N - 1];
        flipCount++;
    }
    bool hasSeenZero = false;
    for (int i = 0; i < N; i++) {
        bool b = bulbs[i];
        bool ans = valForL(i, K, X);
//        std::cerr << "Va " << valForL(i, K, X) << std::endl;
        if (b) {
            oneCount++;
        } else if (!b && oneCount > 0) {
            // 0 came after at least a 1
//            std::cerr << "One count " << oneCount << std::endl;
            if (!hasSeenZero) {
                flipCount += 1;
            } else if (oneCount == 1) {
                std::cerr << "Can not happend " << std::endl;
                abort();
            } else {
                flipCount += 2;
            }
            oneCount = 0;
        }

        if (!b) {
            hasSeenZero = true;
        }
    }
    if (oneCount > 0) {
        if (oneCount == 1) {
            // add One
            std::cerr << "Can not happend " << std::endl;
            abort();
            flipCount += 2;
        } else {
            flipCount += 2; // using flip-flip
        }
    }
    std::cout << flipCount << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}