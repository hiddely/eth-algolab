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

//std::map<std::pair<int, int>, CoolInt> lookup;
std::vector<std::vector<int>> lookupTable;
std::vector<int> coins;


int f(int a, int b) {
    if (a == b) {
        return coins[a];
    }
//    if ()
    if (a + 1 == b) {
        return std::max(coins[a], coins[b]);
    }
    if (lookupTable[a][b] != -1) {
        return lookupTable[a][b];
    }
    int leftFriendPicksLeft = coins[a] + f(a + 2, b);
    int leftFriendPicksRight = coins[a] + f(a + 1, b - 1);
    int rightFriendPicksLeft = coins[b] + f(a + 1, b - 1);
    int rightFriendPicksRight = coins[b] + f(a, b - 2);
    int left = std::min(leftFriendPicksLeft, leftFriendPicksRight);
    int right = std::min(rightFriendPicksLeft, rightFriendPicksRight);
    int max = std::max(left, right);
    lookupTable[a][b] = max;
    return max;
}

void testcase() {
    int n; std::cin >> n;
    coins = std::vector<int>(n);
    for (int i = 0; i < n; i++) {
        std::cin >> coins[i];
    }
    lookupTable = std::vector<std::vector<int>>(n, std::vector<int>(n, -1));

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
