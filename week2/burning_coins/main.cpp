//
// Created by Hidde Lycklama on 9/24/18.
//

#include <iostream>
#include <vector>
#include <map>
#include <utility>

struct CoolInt {
    int val;

    CoolInt(int v): val(v) {}
    CoolInt() { val = -1; }
};

std::map<std::pair<int, int>, CoolInt> lookup;
std::vector<int> coins;


int f(int a, int b) {
    if (a == b) {
        return coins[a];
    }
    if (a + 1 == b) {
        return std::max(coins[a], coins[b]);
    }
    if (lookup[std::pair<int, int>(a, b)].val != -1) {
        return lookup[std::pair<int, int>(a, b)].val;
    }
    int leftFriendPicksLeft = coins[a] + f(a + 2, b);
    int leftFriendPicksRight = coins[a] + f(a + 1, b - 1);
    int rightFriendPicksLeft = coins[b] + f(a + 1, b - 1);
    int rightFriendPicksRight = coins[b] + f(a, b - 2);
    int left = std::min(leftFriendPicksLeft, leftFriendPicksRight);
    int right = std::min(rightFriendPicksLeft, rightFriendPicksRight);
    int max = std::max(left, right);
    lookup[std::pair<int, int>(a, b)] = CoolInt(max);
    return max;
}

void testcase() {
    lookup.clear();
    int n; std::cin >> n;
    coins = std::vector<int>(n);
    for (int i = 0; i < n; i++) {
        std::cin >> coins[i];
    }

    std::cout << f(0, n - 1) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
