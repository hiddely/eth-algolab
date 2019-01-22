#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <numeric>

void genCombs(int i, std::vector<bool> cur, int sum, int max, int len, std::vector<std::vector<bool> > &output) {
    if (i == len) {
        output.push_back(cur);
        return;
    }
    if (sum == max) {
        output.push_back(cur);
        return;
    }
// pick or dont
// diont
    genCombs(i + 1, cur, sum, max, len, output);
    cur[i] = true;
    genCombs(i + 1, cur, sum + 1, max, len, output);
}

void testcase() {
    int n, k;
    std::cin >> n >> k;

    std::vector<int> skills(n);
    for (int i = 0; i < n; i++) {
        int s; std::cin >> s;
        skills[i] = s;
    }

    const int firstHalf = n / 2;
    const int secondHalf = n - firstHalf;

    std::vector<std::vector<bool > > combs;
    std::vector<bool> cur(firstHalf, false);
    genCombs(0, cur, 0, k, firstHalf, combs);

    std::vector<std::vector<int> > sums(k + 1); // K, Sum
    for (int i = 0; i < combs.size(); i++) {
        int refs = 0;
        int curOpt = 0;
        std::vector<int> points;
        for (int r = 0; r < firstHalf; r++) {
            if (combs[i][r]) {
                refs++;
                curOpt++;
            } else {
                points.push_back(curOpt);
                curOpt++;
            }
        }
        for (int s = 0; s < 1 << (firstHalf - refs); s++) {
            int sum = 0;
            for (int x = 0; x < (firstHalf - refs); x++) {
                if (combs[i][points[x]]) { // exclude
                } else
                if (s & (1 << x)) {
                    sum += skills[points[x]]; // red
                } else {
                    sum -= skills[points[x]]; // blue
                }
            }
            // std::cerr << refs << " " << sum << std::endl;
            sums[k - refs].push_back(sum);
        }

    }

    for (int i = 0; i < k + 1; i++) {
        std::sort(sums[i].begin(), sums[i].end());
    }

    if (firstHalf != secondHalf) {
        combs = std::vector<std::vector<bool> >();
        std::vector<bool> cur(secondHalf, false);
        genCombs(0, cur, 0, k, secondHalf, combs);
    }


    // std::cerr << "Sec" << std::endl;
    // for (int i = 0; i < combs.size(); i++) {
    // 	  for (int a = 0; a < secondHalf; a ++ ) {
    // 	       std::cerr << combs[i][a] << " ";
    // 	  }
    // 	  std::cerr << std::endl;
    // }
    // now for the secondhalf
    long count = 0;
    for (int i = 0; i < combs.size(); i++) {
        int refs = 0;
        std::vector<int> points;
        int curPoint = 0;
        for (int r = 0; r < secondHalf; r++) {
            if (combs[i][r]) {
                // std::cerr << 1 << " ";
                refs++;
                curPoint++;
            } else {
                // std::cerr << 0 << " ";
                points.push_back(curPoint);
                curPoint++;
            }
        }
        // std::cerr << std::end ;
        for (int s = 0; s < 1 << (secondHalf - refs); s++) {
            int sum = 0;
            for (int x = 0; x < (secondHalf - refs); x++) {
                if (combs[i][points[x]]) { // exclude
                } else
                if (s & (1 << x)) {
                    sum -= skills[firstHalf + points[x]]; // red
                } else {
                    sum += skills[firstHalf + points[x]]; // blue
                }
            }

            for (int r = refs; r <= k; r++) {
                auto pp = std::equal_range(sums[r].begin(), sums[r].end(), sum);
                auto diff = std::distance(pp.first, pp.second);
                if (diff > 0) {

                    // std::cerr << "Found match " << refs << " " << sum << ": " << diff << std::endl;
                }
                count += diff;
            }


        }

    }
    std::cout << count << std::endl;
    //
    // for (int s = 0; s < 1 << firstHalf; s++) {
    // 	  for (int
    // }


}

int main() {
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}