//
// Created by Hidde Lycklama on 9/21/18.
//

#include <iostream>

void testcase() {
    int n; std::cin >> n;
    int sum = 0;
    for (int i = 0; i < n; i++) {
        int a; std::cin >> a;
        sum += a;
    }
    std::cout << sum << std::endl;
}

int main() {
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
