//
// Created by Hidde Lycklama on 12/28/18.
//

#include <iostream>
#include <vector>

bool valForL(int i, int K, int X) {
    int a = K - (i % K) - 1;
    // now find a'th bit of X;
//    std::cerr << std::bitset<32>(X) << " ";
//    std::cerr << std::bitset<32>(X >> a) << " ";
//    std::cerr << a << " " << i << " " << K << " ";
    int b = (int) (X >> a) & 1;
//    std::cerr << std::bitset<32>(b) << " " << std::endl;
    return (bool)b;
}

void testcase() {

    int N, K, X;
    std::cin >> N >> K >> X;

    int totalCount = 0;
//    int numMovesRequired = 0;
    int oneCount = 0;
    std::vector<std::vector<int>> bulbs(N, std::vector<int>(2)); // Stores the states of the bulbs up to i
//    std::vector<bool> bulbs(N);
    for (int i = 0; i < N; i++) {
        bool b; std::cin >> b;
        bool ans = valForL(i, K, X);
        bool is_correct = !(b ^ ans);
        if (i == 0) {
            bulbs[i][is_correct] = 0;
            bulbs[i][!is_correct] = 1; // switch
        } else {
            bulbs[i][is_correct] = bulbs[i - 1][is_correct];
            bulbs[i][!is_correct] = bulbs[i - 1][!is_correct] + 1; // switch
        }
        if (i % K == K - 1) {
            // edge, we may swap it all
            bulbs[i][0] = std::min(bulbs[i][0], bulbs[i][1] + 1);
            bulbs[i][1] = std::min(bulbs[i][1], bulbs[i][0] + 1);
        }
//        std::cerr << b << " ^ " << ans << " = " << bulbs[i];
    }

    std::cout << bulbs[N - 1][1] << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}