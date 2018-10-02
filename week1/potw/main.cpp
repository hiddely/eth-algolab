//
// Created by Hidde Lycklama on 9/21/18.
//

#include <iostream>
#include <vector>
#include <limits>

void testcase() {
    int n; std::cin >> n;
    int k; std::cin >> k;
    std::vector<int> nums(n);
    std::vector<int> sums(n);
    for (int i = 0; i < n; i++) {
        int number; std::cin >> number;
        nums[i] = number;
        if (i == 0) {
            sums[i] = nums[i];
        } else {
            sums[i] = sums[i - 1] + nums[i];
        }
    }

    int x = 0, y = 0;
    int cX = -1, cY = -1;
    int closest = std::numeric_limits<int>::max();
    int current = nums[0];
    while (x < n || y < n) {
        // Add the next
        int diff = abs(k - current);
        if (diff < closest) {
//            std::cout << "Updating " << diff << " " << closest << std::endl;
            closest = diff;
            cX = x;
            cY = y;
        }
        if (x == y) {
            // Just keep going
            y++;
            current += nums[y];
        } else if (current < k && y < n) {
            // Increase, we need to add more
            y++;
            current += nums[y];
        } else {
            // Decrease, we need to remove
            current -= nums[x];
            x++;
        }
    }
    std::cout << cX << " " << cY << std::endl;
}

int main() {
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
