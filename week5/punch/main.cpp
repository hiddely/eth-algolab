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
        if (this->ratio() < o.ratio()) {
            return this->volume > o.volume; // higher volume first
        }
        return this->ratio() < o.ratio();
//        return this->ratio() < o.ratio();
    }
};

typedef std::pair<long, long> Result; // COST, NUMDIFF

typedef std::vector<std::vector<Result> > Table;

Table DP;

//Result KNAP(int i, int curvol, int vol, std::vector<Beverage> &drinks) {
//    if (curvol >= vol) {
//        // we are done
//        return Result(0, 0);
//    }
//    if (i == -1) {
//        // we are done
//        return Result(INT_MAX, 0);
//    }
//    if (DP[i][curvol] != Result(-1, -1)) {
//        return DP[i][curvol];
//    }
//
//
//    // fill the bottom
//    Result pick = KNAP(i, curvol + drinks[i].volume, vol, drinks);
////    std::cerr << "pick " << pick.first << " " << pick.second << " " << lastpick << " " << i << std::endl;
//
////    int add = 0;
////    if (DP[i + 1][curvol + drinks[i].volume] == DP[i][curvol + drinks[i].volume]) {
////        add = 1;
////    }
//
//    Result pickAdd = Result(drinks[i].cost + pick.first, pick.second + add);
//
//    Result dontpick = KNAP(i - 1, curvol, vol, drinks);
//
//    Result val;
//    if (pickAdd.first == dontpick.first) {
//        val = pickAdd; // always pick !!!
//    } else if (pickAdd.first < dontpick.first) {
//        val = pickAdd;
//    } else {
//        val = dontpick;
//    }
//    DP[i][curvol] = val;
//    return val;
//}

void testcase() {
    int n; int people;
    std::cin >> n >> people;
    std::vector<Beverage> drinks(n);
    for (int i = 0; i < n; i++) {
        int c; int v;
        std::cin >> c >> v;
//        drinks[i] = Beverage(c, v);
//        for (int a = 0; a < people / v;)
        Beverage b = Beverage(c, v);
        drinks[i] = b;
    }

    DP = Table(n, std::vector<Result>(people, Result(-1, -1)));

    std::vector<std::vector<Result > > map(people + 1, std::vector<Result>(n, Result(-1, -1))); // Beverages / volume -> Cost

    for (int v = 0; v < people; v++) {
        // start building up v
        for (int i = 0; i < n; i++) {
            long minCost;
            long minChoice;
            if (drinks[i].volume > v) {
                // add this one
                minCost = drinks[i].cost;
                minChoice = 1;
            } else {
                Result down = map[v - drinks[i].volume][i];
                minCost = down.first + drinks[i].cost;
                minChoice = down.second;
                if (i > 0 && down == map[v - drinks[i].volume][i - 1] ? 1 : 0) {
                    minChoice += 1;
                }
                // if lower same as prev bev, we havent used this beverage yet
            }
//            std::cerr << v << " " << i << ": " << minCost << " " << minChoice << std::endl;
            if (i > 0) {
                Result left = map[v][i - 1];
                if (left.first < minCost) {
                    minCost = left.first;
                    minChoice = left.second;
                } else if (left.first == minCost) {
                    if (left.second > minChoice) {
                        minChoice = left.second;
                    }
                }
            }

            map[v][i] = Result(minCost, minChoice);
        }
    }

    std::cout << map[people - 1][n - 1].first << " " << map[people - 1][n - 1].second << std::endl;
//

//    Result VAL = KNAP(n - 1, 0, people, drinks);
//
//    std::cout << VAL.first << " " << VAL.second << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t > 0) {
        testcase();
        t--;
    }
}