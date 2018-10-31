//
// Created by Hidde Lycklama on 10/27/18.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <set>
#include <stack>
#include <map>
#include <cmath>
#include <tuple>
#include <deque>

struct Beverage {
    int cost;
    int volume;

    Beverage(int c, int v): cost(c), volume(v) {}
    Beverage() = default;

    long ratio() const {
        long r = std::round(((double)cost / (double)volume) * 1e8);
        if (r <= 0) {
            std::cout << "AYY " << r << std::endl;
        }
        return r;
    }

    bool operator < (const Beverage &o) const {
        if (this->cost == o.cost) {
            return this->volume < o.volume;
        }
        return this->cost < o.cost;
//        return this->ratio() < o.ratio();
    }
};

std::pair<int, int> cost(int l, int uniques, int targetL, std::set<Beverage> &drinks, std::vector<std::vector<std::pair<int, int> > > &dp, std::deque<long> &hist, std::vector<Beverage> &steps, std::map<long, int> &maxRatios, int max) {
    if (l >= targetL) {
//        std::stack<Beverage> x = hist;
//        while (!x.empty()) {
//            std::cout << "Hist: " << x.top().cost << " " << x.top().volume << std::endl;
//            x.pop();
//        }
        return std::pair<int, int>(0, uniques);
    }
    if (dp[l][uniques] != std::pair<int, int>(0, 0)) {
//        std::cout << " Returning " << uniques << std::endl;
        return dp[l][uniques];
    }
    auto it = drinks.begin();
    std::pair<int, int> min = std::pair<int, int>(INT_MAX, 0);
    Beverage b;
    for (; it != drinks.end(); it++) {
        int u = uniques;
        if (std::find(hist.begin(), hist.end(), it->ratio()) == hist.end()) {
            u++;
        } else {
//            std::cout << "Already found " << it->ratio() << std::endl;
        }
        hist.push_back(it->ratio());
        std::pair<int, int> calc = cost(l + it->volume, u, targetL, drinks, dp, hist, steps, maxRatios, max);
        hist.pop_back();
        int c = calc.first + it->cost;
        if (c < min.first || (c > 0 && c != INT_MAX && c == min.first && calc.second > min.second)) {
            min = std::pair<int, int>(c, calc.second);
            b = *it;
        }
    }
    steps[l] = b;
//    if (uniques >= max) {
//        std::cout << "ERRORR TOO BIG " << uniques << std::endl;
//        throw 1;
//    }
//    std::cout << uniques << std::endl;
    dp[l][uniques] = min;
    return min;
}

void testcase(int t) {
    int n; int people;
    std::cin >> n >> people;
    std::vector<Beverage> drinks(n);
    std::set<Beverage> d;
    std::map<long, int> maxRatios;
    for (int i = 0; i < n; i++) {
        int c; int v;
        std::cin >> c >> v;
        drinks[i] = Beverage(c, v);
        d.insert(Beverage(c, v));
        maxRatios[drinks[i].ratio()]++;
    }
//
//    if (t != 10) {
//        return;
//    }

//    std::sort(drinks.begin(), drinks.end());

    std::vector<std::vector<std::pair<int, int> > > dp(people, std::vector<std::pair<int, int> >(2 * n, std::pair<int, int>(0, 0)));
    std::deque<long> hist;
    std::vector<Beverage> steps(people);

    std::pair<int, int> c = cost(0, 0, people, d, dp, hist, steps, maxRatios, n);

//    for (int a = 0; a< people; a++) {
//        for (int b = 0; b < n; b++) {
//            std::cout << dp[a][b].first << " ";
//        }
//        std::cout << std::endl;
//    }

    std::map<long, int> ratios;
    int i = 0;
    int newCost = 0;
    while (i < people) {
//        std::cout << "Step " << i << " ";
//        std::cout << steps[i].volume << " " << steps[i].cost << " " << steps[i].ratio() << std::endl;
        ratios[steps[i].ratio()]++;
        i += steps[i].volume;
        newCost += steps[i].cost;
    }

    // Limit to max occurrences of that ratio
    int numDiffBev = 0;
    for (auto it = ratios.begin(); it != ratios.end(); it++) {
//        std::cout << "F " << it->first << std::endl;
        numDiffBev += std::min(it->second, maxRatios[it->first]);
    }

    std::cout << c.first << " " << numDiffBev << std::endl;

//    for (int i = 0; i < steps.size(); i++) {
//        std::cout << "N " << i << " " << steps[i].cost << " " << steps[i].volume << std::endl;
//    }

//    int minCount = 0;
//    double minRatio = drinks.begin()->ratio();
//    for (int i = 0; i < n; i++) {
//        if (drinks[i].ratio() == minRatio) {
//            minCount++;
//        } else {
//            break;
//        }
//    }
//
//    int neededAmount = drinks.begin()->ratio() * people;
//    std::cout << neededAmount << " " << minCount << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t > 0) {
        testcase(t);
        t--;
    }
}