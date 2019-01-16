//
// Created by Hidde Lycklama on 9/24/18.
//

#include <iostream>
#include <vector>

int binomial_coefficient_pick_two(int n) {
//    assert(n > 2);
    return n * (n - 1) / 2;
}

int binomial_coefficient_pick_four(int n) {
//    assert(n > 2);
    return n * (n - 1) * (n - 2) * (n - 3) / 24;
}

void testcase() {
    int n; std::cin >> n;
    std::vector<std::vector<int>> nums(n); // x , y
    for (int i = 0; i < n; i++) {
        nums[i] = std::vector<int>(n);
        for (int j = 0; j < n; j++) {
            std::cin >> nums[i][j];
        }
    }

    std::vector<std::vector<int>> partials(n);
    for (int i = 0; i < n; i++) {
        partials[i] = std::vector<int>(n);
        for (int j = 0; j < n; j++) {
            if (i == 0 && j == 0) {
                partials[i][j] = nums[i][j];
            } else if (i == 0 && j != 0) {
                partials[i][j] = nums[0][j] + partials[0][j - 1];
            } else if (i != 0 && j == 0) {
                partials[i][j] = partials[i - 1][0] + nums[i][0];
            } else {
                partials[i][j] = partials[i - 1][j] + partials[i][j - 1] + nums[i][j] - partials[i - 1][j - 1];
            }
        }
    }

    int solution = 0;
    for (int i1 = 0; i1 < n; i1++) {
        for (int i2 = i1; i2 < n; i2++) {
            std::vector<int> S(n);
            std::vector<int> pS(n);
            for (int k = 0; k < n; k++) {
                if (k == 0 && i1 == 0) {
                    S[k] = partials[i2][k];
                    pS[k] = S[k];
                } else if (k == 0) {
                    S[k] = partials[i2][k] - partials[i1 - 1][k];
                    pS[k] = S[k];
                } else if (i1 == 0) {
                    S[k] = partials[i2][k] - partials[i2][k - 1];
                    pS[k] = pS[k - 1] + S[k];
                } else {
                    S[k] = partials[i2][k] - partials[i2][k - 1] - partials[i1 - 1][k] + partials[i1 - 1][k - 1];
                    pS[k] = pS[k - 1] + S[k];
                }
            }

            int even = 0; int odd = 0;
            for (int j = 0; j < n; j++) {
                if (pS[j] % 2 == 0) {
                    even++;
                } else {
                    odd++;
                }
            }

            solution += binomial_coefficient_pick_two(even);
            solution += binomial_coefficient_pick_two(odd);
            solution += even;
        }
    }
    std::cout << solution << std::endl;

//    for (int i1 = 0; i1 < n; i1++) {
//        for (int i2 = i1; i2 < n; i2++) {
//            for (int j1 = 0; j1 < n; j1++) {
//                for (int j2 = j1; j2 < n; j2++) {
//
//                    int sum = 0;
//                    if (i1 == 0 && j1 == 0) {
//                        sum = partials[i2][j2]; // All evens
//                    } else if (i1 == 0) {
//                        sum = partials[i2][j2] - partials[i2][j1 - 1]; // Even choose 2, uneven choose 2
//                    } else if (j1 == 0) {
//                        sum = partials[i2][j2] - partials[i1 - 1][j2]; // same as above
//                    } else {
//                        sum = partials[i2][j2] - partials[i2][j1 - 1] - partials[i1 - 1][j2] + partials[i1 - 1][j1 - 1];
//                    }
//
//                    if (sum % 2 == 0) {
//                        even++;
//                    } else {
//                        odd++;
//                    }
//                }
//            }
//        }
//    }
//    std::cout << even << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
