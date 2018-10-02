//
// Created by Hidde Lycklama on 9/24/18.
//

#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <utility>

struct CoolInt {
    double val;

    CoolInt(double val) : val(val) {}

    CoolInt() { val = -1; }
};

std::map<std::pair<int,int>, CoolInt> lookup;
std::vector<double> probabilities;
int days;

double prob(int start, int target, int index) {
    if (start >= target) {
        return 1;
    }
    if (start <= 0) {
        return 0;
    }
    if (index == days) {
        return 0; // Too slow
    }
    if (lookup[std::make_pair(start,index)].val != -1) {
        return lookup[std::make_pair(start,index)].val;
    }
    double max = 0;
    double p = probabilities[index];
    for (int i = 0; i <= start; i++) {
        double win = p * prob(start + i, target, index + 1);
        if (win == 0) {
            continue;
        }
        double lose = (1 - p) * prob(start - i, target, index + 1);
        double result = win + lose;
        if (result > max) {
            max = result;
        }
    }
    lookup[std::make_pair(start,index)] = CoolInt(max);
    return max;
}

void testcase() {
    lookup.clear();
    std::cin >> days;
    int start; std::cin >> start;
    int target; std::cin >> target;

    probabilities = std::vector<double>(days);
    for (int i = 0; i < days; i++) {
        std::cin >> probabilities[i];
    }

    double p = prob(start, target, 0);

    std :: cout << std :: fixed << std::setprecision (5);
    std::cout << p << std::endl; // Replace 3.5 with your desired double

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
