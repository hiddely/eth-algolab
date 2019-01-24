#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <iomanip>

typedef std::vector<std::vector<double> > Table;

Table DP;

double calc(int day, int money, int num, int targ, std::vector<double> &probs) {
    if (money >= targ) {
        // quit?
        return 1;
    }
    if (day == num) {
        return (double)(money >= targ);
    }
    if (DP[day][money] != -1) {
        return DP[day][money];
    }
    double max = 0;
    for (int i = 0; i <= money; i++) {
        // place bet for i
        double success = probs[day] * calc(day + 1, money + i, num, targ, probs);
        double fail = (1 - probs[day]) * calc(day + 1, money - i, num, targ, probs);
        //std::cerr << day << ": Bet " << i << " " << (success + fail) << ", " << probs[day] << std::endl;
        max = std::max(max, success + fail);
    }
    DP[day][money] = max;
    return max;
}


void testcase() {

    int n, k, m;
    std::cin >> n >> k >> m;
    std::vector<double> probs(n);
    for (int i = 0; i < n; i++) {
        double p; std::cin >> p;
        probs[i] = p;
    }

    DP = Table(n, std::vector<double>(2 * m + 2, -1));

    std::cout << std::fixed << std::setprecision(5);
    std::cout << calc(0, k, n, m, probs) << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}