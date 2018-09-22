//
// Created by Hidde Lycklama on 9/21/18.
//

#include <iostream>
#include <string>
#include <vector>
#include <boost/math/special_functions/binomial.hpp>

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
    double binEven = even / 2;
    double binOdd = odd / 2;
    if (even > 2) {
        binEven = boost::math::binomial_coefficient<double>((unsigned int)even, 2);
    }
    if (odd > 2) {
        binOdd = boost::math::binomial_coefficient<double>((unsigned int)odd, 2);
    }
    int res = ((int)binEven) + ((int)binOdd) + even;
    std::cout << res << std::endl;
}

int main() {
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
