//
// Created by Hidde Lycklama on 9/21/18.
//

#include <iostream>
#include <vector>

//long double factorial(unsigned int n) {
//    if (n == 0) {
//        return 1;
//    }
//    return ((long double)n) * factorial(n - 1);
//}
//
//long double binomial_coefficient(int n, int k) {
//    return factorial(n) / (factorial(k) * factorial(n - k));
//}
// Actually, we can optimize the above, since k is always two.

// Assume that n > 2
int binomial_coefficient_pick_two(int n) {
//    assert(n > 2);
    return n * (n - 1) / 2;
}

void testcase() {
    int n; std::cin >> n;
    std::vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        int number; std::cin >> number;
        nums[i] = number;
    }
    std::vector<int> partials(n);
    int even = 0;
    int odd = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            partials[0] = nums[0];
        } else {
            partials[i] = nums[i] + partials[i - 1];
        }
        if (partials[i] % 2 == 0) {
            even++;
        } else {
            odd++;
        }
    }
    int binEven = even / 2;
    int binOdd = odd / 2;
    if (even > 2) {
        binEven = binomial_coefficient_pick_two(even);
    }
    if (odd > 2) {
        binOdd = binomial_coefficient_pick_two(odd);
    }
    int res = binEven + binOdd + even;
    std::cout << res << std::endl;
}

int main() {
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
