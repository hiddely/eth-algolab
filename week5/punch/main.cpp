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

struct cmp_rat {
    bool operator ()(const Beverage &o1, const Beverage &o2) const {
        return o1.ratio() < o2.ratio();
    }
};

bool compare_ratio(const Beverage &o1, const Beverage &o2) {
    if (o1.ratio() == o2.ratio()) {
        return o1.volume < o2.volume;
    }
    return o1.ratio() < o2.ratio();
}

void testcase(int t) {
    int n; int people;
    std::cin >> n >> people;
    std::vector<Beverage> drinks(n);
    std::set<Beverage> dRatio;
    std::map<long, int> maxRatios;
    for (int i = 0; i < n; i++) {
        int c; int v;
        std::cin >> c >> v;
        drinks[i] = Beverage(c, v);
        dRatio.insert(Beverage(c, v));
        maxRatios[drinks[i].ratio()]++;
    }

    // Sorted by ratio ("cheapest") then smallest volume
    std::sort(drinks.begin(), drinks.end(), compare_ratio);

    int left = people;
    int distinct = 0;
    int cost = 0;
    std::map<long, int> ratiosUsed;
    auto first = drinks[0];
    while (left >= first.volume) {
        ratiosUsed[first.ratio()]++;
        cost += first.cost;
        left -= first.volume;
//        std::cout << "Subbing " << first.cost << " " << first.volume << std::endl;
    }

//    int toDivide = first.volume - left; // How much 'width' do we have?
//    for (int i = 0; i < n; i++) {
//        // what drinks are lower than what we need but want more?
//        int timesNeeded = ceil((double)left / (double)drinks[i].volume);
//        int costToCover = timesNeeded * drinks[i].cost;
//
//    }

    int min = INT_MAX;
    Beverage b;
    int times = 0;
    for (int i = 0; i < n; i++) {
        int t = ceil((double)left / (double)drinks[i].volume);
        int costNeeded = t * drinks[i].cost;
        if (costNeeded < min || (costNeeded == min && ratiosUsed[drinks[i].ratio()] < ratiosUsed[b.ratio()])) {
            min = costNeeded;
            b = drinks[i];
            times = t;
//            std::cout << "Settings to " << costNeeded << " " << left << " " << drinks[i].volume << std::endl;
        }
    }
    ratiosUsed[b.ratio()] += times;
    cost += min;
//    for (auto iter = drinks.begin(); iter != drinks.end(); iter++) {
//        while (left >= iter->volume) {
//            ratiosUsed[iter->ratio()]++;
//            cost += iter->cost;
//            left -= iter->volume;
//            std::cout << "Subbing " << iter->cost << " " << iter->volume << std::endl;
//        }
//    }
//    if (left > 0) {
//        auto cheapest = std::min_element(drinks.cbegin(), drinks.cend());
//        left -= cheapest->volume;
//        cost += cheapest->cost;
////        std::cout << "Left is bigger " << left <<std::endl;
//    }

    int numDiffBev = 0;
    for (auto it = ratiosUsed.begin(); it != ratiosUsed.end(); it++) {
//        std::cout << "F " << it->first << std::endl;
        numDiffBev += std::min(it->second, maxRatios[it->first]);
    }

    std::cout << cost << " " << numDiffBev << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t > 0) {
        testcase(t);
        t--;
    }
}