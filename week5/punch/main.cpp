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

struct Beverage {
    int cost;
    int volume;

    Beverage(int c, int v): cost(c), volume(v) {}
    Beverage() = default;

    double ratio() const {
        return (double)cost / (double)volume;
    }

    bool operator < (const Beverage &o) const {
        if (this->cost == o.cost) {
            return this->volume < o.volume;
        }
        return this->cost < o.cost;
//        return this->ratio() < o.ratio();
    }
};

int cost(int l, int targetL, std::set<Beverage> &drinks, std::vector<int> &dp, std::stack<Beverage> &hist, std::map<int, Beverage> &steps) {
    if (l >= targetL) {
//        std::stack<Beverage> x = hist;
//        while (!x.empty()) {
//            std::cout << "Hist: " << x.top().cost << " " << x.top().volume << std::endl;
//            x.pop();
//        }
        return 0;
    }
//    if (dp[l] != 0) {
//        return dp[l];
//    }
    auto it = drinks.begin();
    int min = INT_MAX;
    Beverage b;
    for (; it != drinks.end(); it++) {
        hist.push(*it);
        int calc = cost(l + it->volume, targetL, drinks, dp, hist, steps);
        hist.pop();
        int c = calc + it->cost;
        if (c < min) {
            min = c;
            b = *it;
        }
    }
    steps[l] = b;
    dp[l] = min;
    return min;
}

void testcase(int t) {
    int n; int people;
    std::cin >> n >> people;
    std::vector<Beverage> drinks(n);
    std::set<Beverage> d;
    for (int i = 0; i < n; i++) {
        int c; int v;
        std::cin >> c >> v;
        drinks[i] = Beverage(c, v);
        d.insert(Beverage(c, v));
    }
//
//    if (t != 4) {
//        return;
//    }

//    std::sort(drinks.begin(), drinks.end());

    std::vector<int> dp(people);
    std::stack<Beverage> hist;
    std::map<int, Beverage> steps;

    std::cout << cost(0, people, d, dp, hist, steps) << std::endl;

    auto it = steps.begin();
    for (; it != steps.end(); it++) {
        auto st = *it;
        std::cout << "N " << st.first << " " << st.second.cost << " " << st.second.cost << std::endl;
    }

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