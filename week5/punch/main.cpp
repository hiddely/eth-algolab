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

struct my_cmp {
    bool operator () (const Beverage &b1, const Beverage &b2) const {
        return b1.ratio() < b2.ratio();
    }
};

void testcase(int t) {
    int n; int people;
    std::cin >> n >> people;
    std::vector<Beverage> drinks(n);
//    std::set<Beverage> d;
    std::map<long, int> maxRatios;
    for (int i = 0; i < n; i++) {
        int c; int v;
        std::cin >> c >> v;
//        drinks[i] = Beverage(c, v);
        Beverage b = Beverage(c, v);
        drinks[i] = b;
//        d.insert(b);
        maxRatios[b.ratio()]++;
    }

    std::sort(drinks.begin(), drinks.end(), my_cmp());

    // get cheapest price
    Beverage *cheapest = &drinks[0];
    std::cerr << "Cheapest " << cheapest->cost << " " << cheapest->volume << std::endl;
    int lowPrice = (people / cheapest->volume) * cheapest->cost;
    std::cout << lowPrice << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t > 0) {
        testcase(t);
        t--;
    }
}