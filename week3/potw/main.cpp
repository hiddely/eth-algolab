//
// Created by Hidde Lycklama on 10/10/18.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <climits>

typedef std::pair<int, int> Option;

//int calc(int a, int b, int sum, int numAttack, int k, int n, std::vector<int> defense) {
//    if (numAttack == 0) {
//        return 0;
//    }
//    if (a == n || b == n + 1) {
//        return -100000000;
//    }
//    std::pair<std::pair<std::pair<int, int>, int>, int> key
//        = std::pair<std::pair<std::pair<int, int>, int>, int>(std::pair<std::pair<int, int>, int>(std::pair<int, int>(a, b), sum), numAttack);
//    if (DP[key].val != -1) {
//        return DP[key].val;
//    }
//    if (sum < k) {
//        // just increment the window
////        std::cerr << "Increment " << defense[b] << " " << sum + defense[b] << std::endl;
//        return calc(a, b + 1, sum + defense[b], numAttack, k, n, defense);
//    }
//    if (sum > k) {
//        // decrement the window
////        std::cerr << "decrement " << defense[a] << " " << sum - defense[a] << std::endl;
//        return calc(a + 1, b, sum - defense[a], numAttack, k, n, defense);
//    }
//    // sum == k
////    std::cerr << "Match " << a << " " << b << std::endl;
//    int apply = (b - a) + calc(b, b + 1, defense[b], numAttack - 1, k, n, defense);
//    int no = calc(a + 1, b, sum - defense[a], numAttack, k, n, defense);
//    int val = std::max(apply, no);
//    DP[key].val = val;
//    return val;
//}

struct NotZero {
    int val = -1;
    NotZero() = default;
};

typedef std::vector<NotZero> P;
typedef std::vector<P> PP;

PP DP;

int calc(int i, int attackers, int n, std::vector<Option> &options) {
    if (attackers == 0) {
//        std::cerr << "Done " << i << std::endl;
        return 0; // finito
    }
    if (i >= n) {
        return INT_MIN;
    }
    if (DP[i][attackers].val != -1) {
        return DP[i][attackers].val;
    }
    auto iter = options[i];
    int pick = INT_MIN;
    if (iter != Option(0, 0)) {
        pick = (iter.second - iter.first) + calc(iter.second, attackers - 1, n, options);
//        std::cerr << i << " OptionPot " << iter.first << " " << iter.second << ", pick: " << pick << " dont " << std::endl;
    }
    int dontpick = calc(i + 1, attackers, n, options);
    int val = std::max(pick, dontpick);
    DP[i][attackers].val = val;
    return val;
}

void testcase() {
    int numDefense; std::cin >> numDefense;
    int numAttack; std::cin >> numAttack;
    int attackStrength; std::cin >> attackStrength;
    std::vector<int> defenseValues(numDefense);
    for (int i = 0; i < numDefense; i++) {
        std::cin >> defenseValues[i];
    }

    DP = PP(numDefense, P(numAttack + 1));

    int sum = 0;
    int a = 0; int b = 0;
    std::vector<Option> options(numDefense);
    while (b <= numDefense && a < numDefense) {
//        std::cerr << "Build " << a << " " << b << std::endl;
        if (sum < attackStrength) {
            // incr
            sum += defenseValues[b];
            b++;
        } else if (sum > attackStrength) {
            // decr
            sum -= defenseValues[a];
            a++;
        } else {
            // sum == attackStrength
            // add as option
//            std::cerr << "Option " << a << " " << b << std::endl;
            options[a] = Option(a, b);
            sum -= defenseValues[a];
            a++;
        }
    }

//    DP = T();

    int res = calc(0, numAttack, numDefense, options);
    if (res < 0) {
        std::cout << "fail" << std::endl;
    } else {
        std::cout << res << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int a = 0; a < t; a++) {
        testcase();
    }
    return 0;
}