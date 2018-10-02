//
// Created by Hidde Lycklama on 9/24/18.
//

#include <iostream>
#include <vector>

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
    int even = 0;
    int odd = 0;
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

    for (int i1 = 0; i1 < n; i1++) {
        for (int i2 = i1; i2 < n; i2++) {
            for (int j1 = 0; j1 < n; j1++) {
                for (int j2 = j1; j2 < n; j2++) {

                    int sum = 0;
                    if (i1 == 0 && j1 == 0) {
                        sum = partials[i2][j2];
                    } else if (i1 == 0) {
                        sum = partials[i2][j2] - partials[i2][j1 - 1];
                    } else if (j1 == 0) {
                        sum = partials[i2][j2] - partials[i1 - 1][j2];
                    } else {
                        sum = partials[i2][j2] - partials[i2][j1 - 1] - partials[i1 - 1][j2] + partials[i1 - 1][j1 - 1];
                    }

                    if (sum % 2 == 0) {
                        even++;
                    } else {
                        odd++;
                    }
                }
            }
        }
    }
    std::cout << even << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
