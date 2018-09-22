//
// Created by Hidde Lycklama on 9/21/18.
//

#include <iostream>
#include <vector>

void testcase() {
    int n; std::cin >> n;
    std::vector<int> heights(n);
    for (int i = 0; i < n; i++) {
        std::cin >> heights[i];
    }
    int h = heights[0]; // trip the first
    int i = 1;
    for (i = 1; i < n; i++) {
        h = h - 1;
        if (h < 1) {
            break;
        }
        h = std::max(h, heights[i]);
    }

    std::cout << i << std::endl;
}

int main() {
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
