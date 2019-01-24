#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

void testcase() {

    int n; std::cin >> n;
    std::vector<std::pair<int, int> > boats(n);
    for (int i = 0; i < n; i++) {
        int l, p;
        std::cin >> l >> p;
        boats[i] = std::pair<int, int>(p, l);
    }
    std::sort(boats.begin(), boats.end());
    int curPos = boats[0].first;
    int prevPos = boats[0].first;
    int numBoats = 1;
    for (int i = 1; i < n; i ++) {
        // can we lay out this boat, or preferably the next one?
        //std::cerr << curPos << " choose between " << boats[i].first << " and " << boats[i + 1].first << std::endl;
        if (boats[i].first < curPos) {
            // check if this option is better
            curPos = std::min(curPos, std::max(prevPos + boats[i].second, boats[i].first));
        } else {
            // lay it out
            prevPos = curPos;
            curPos = std::max(curPos + boats[i].second, boats[i].first);

            numBoats++;
        }
    }
    std::cout << numBoats << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}