//
// Created by Hidde Lycklama on 9/24/18.
//

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <utility>

std::vector<std::vector<double>> lookup;
std::vector<double> probabilities;
int days;
int target;

double prob(int start, int index) {
    if (start >= target) {
        return 1;
    }
    if (start <= 0) {
        return 0;
    }
    if (index == days) {
        return 0; // Too slow
    }
    if (lookup[index][start] != -1) {
        return lookup[index][start];
    }
    double max = 0;
    double p = probabilities[index];
    for (int i = 0; i <= start; i++) {
        double win = p * prob(start + i, index + 1);
        double lose = (1 - p) * prob(start - i, index + 1);
        double result = win + lose;
        if (result > max) {
            max = result;
        }
    }
    lookup[index][start] = max;
    return max;
}

void testcase() {
    std::cin >> days;
    int start; std::cin >> start;
    std::cin >> target;

    probabilities = std::vector<double>(days);
    for (int i = 0; i < days; i++) {
        std::cin >> probabilities[i];
    }

    lookup = std::vector<std::vector<double>>(days, std::vector<double>(target, -1));

    double p = prob(start, 0);

    std::cout << p << std::endl; // Replace 3.5 with your desired double

}

int main() {
    std::ios_base::sync_with_stdio(false);
    std :: cout << std :: fixed << std::setprecision (5);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
