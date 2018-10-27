//
// Created by Hidde Lycklama on 10/21/18.
//

#include <iostream>
#include <vector>
#include <tuple>
#include <climits>
#include <set>
#include <algorithm>

int BitCount(unsigned int u)
{
    unsigned int uCount;

    uCount = u
             - ((u >> 1) & 033333333333)
             - ((u >> 2) & 011111111111);
    return
            ((uCount + (uCount >> 3))
             & 030707070707) % 63;
}

int sum(int room, std::vector<std::vector<std::pair<int, int>>> &switches, int config, int start, int end) {
    int sum = 0;
    for (int a = start; a < end; a++) {
        // enabled if the bit in config is 0 / 1
        if (config & 1 << (a - start)) {
            sum += switches[a][room].first;
        } else {
            sum += switches[a][room].second;
        }
    }
    return sum;
}

struct Sum {
    int s;
    int index;

    Sum() = default;
    Sum(int x, int i): s(x), index(i) {}

    bool operator <(const Sum &o) const {
        return this->s < o.s;
    }
};

struct lex_compare {
    bool operator() (const Sum &p1, const Sum &p2) const {
        return p1.s < p2.s;
    }
};
//
//bool operator < (const Sum &s1, const Sum &s2) {
//    return s1.s < s2.s;
//}

void testcase() {
    int numSwitches; int numRooms;
    std::cin >> numSwitches >> numRooms;

    std::vector<int> target(numRooms);
    for (int i = 0; i < numRooms; i++) {
        std::cin >> target[i];
    }

//    std::cout << "targets" << std::endl;

    std::vector<std::vector<std::pair<int, int>>> switches(numSwitches, std::vector<std::pair<int, int>>(numRooms));
    for (int a = 0; a < numSwitches; a++) {
        for (int b = 0; b < numRooms; b++) {
            int on, off;
            std::cin >> on >> off;
//            std::cout << a << " " << b / 2 << std::endl;
            switches[a][b] = std::pair<int, int>(on, off);
        }
    }

//    for (int b = 0; b < numRooms; b++) {
//        for (int a = 0; a < numSwitches; a++ ){
//            std::cout << switches[a][b].first << " " << switches[a][b].second << " ";
//        }
//        std::cout << std::endl;
//    }

    int split1 = numSwitches / 2;
    int split2 = numSwitches - split1;
    int count1 = 1 << split1;
    int count2 = 1 << split2;

    std::vector<std::vector<Sum> > sum1(numRooms, std::vector<Sum>(count1));
    std::vector<std::vector<Sum> > sum2(numRooms, std::vector<Sum>(count2));
    for (int b = 0; b < numRooms; b++){
        for (int a = count1 - 1; a >= 0; a--) {
            int s = sum(b, switches, a, 0, split1);
//            std::bitset<16> on(a);
//            std::cout << a << ": s(" << s << ") " << on << std::endl;
//            std::cout << split1 - BitCount(a) << ": " << s << std::endl;
            sum1[b][a] = Sum(s, a);
        }
        for (int a = count2 - 1; a >= 0; a--) {
            int s = sum(b, switches, a, split1, numSwitches);
//            if (s != 866) {
//if (a == 32509) {
//    std::bitset<16> on(a);
//    std::cout << a << " Adding s " << s << " " << on << " " << split1 << " " << numSwitches << std::endl;
//
//}
//            }
//            std::cout << split2 - BitCount(a) << ": " << s << std::endl;

            sum2[b][a] = Sum(s, a);
        }
        std::sort(sum1[b].begin(), sum1[b].end());
        std::sort(sum2[b].begin(), sum2[b].end());
    }

//    for (int a = split; a < numSwitches; a++ ){
//        std::cout << "Switch: " << a << ": " << switches[a][0].first << " " << switches[a][0].second << std::endl;
//    }

//    std::vector<std::set<std::pair<int, int>, lex_compare> > choices(numRooms);
//    for (int b = 0; b < numRooms; b++) {
//        for (int a = count; a > 0; a--) { // the choice
//            int s1 = sum1[b][a];
//            int diff = target[b] - s1;
//            auto elem = std::lower_bound(sum2[b].cbegin(), sum2[b].cend(), diff); // should we find other options too?
//            if (*elem == diff) {
//                std::cout << "Match ! " << s1 << " + " << *elem << " = " << target[0] << std::endl;
////                std::cout << a << " " << elem << std::endl;
//                choices[b].insert(std::pair<int, int>(a, elem));
//            }
//        }
//    }
//
//    // Configs for the rooms have to match...
//    for (int b = 0; b < numRooms; b++) {
//
//    }

    int i = 0; int j = count2 - 1;
    int maxSwitches = INT_MAX;
    while (i < count1 && j >= 0) {

        std::vector<int> matches(numRooms);
        for (int r = 0; r < numRooms; r++) {
            matches[r] = sum1[r][i].s + sum2[r][j].s;

        }
        int numSwitchesNeeded = (split1 - BitCount(sum1[0][i].index)) + (split2 - BitCount(sum2[0][j].index));
//std::cout << "NumSwitches " << numSwitchesNeeded <<std::endl;
        if (matches == target) {
            if (numSwitchesNeeded < maxSwitches) {
                maxSwitches = numSwitchesNeeded;
            }
            if (j > 0 && sum2[0][j].s == sum2[0][j - 1].s) {
                j--;
            } else if (i < count1 - 1 && sum1[0][i].s == sum1[0][i + 1].s) {
                i++;
            } else if (j > 0) {
                j--;
            } else {
                i++;
            }
        } else if (matches < target) {
            i++;
        } else {
            j--;
        }
    }
    if (maxSwitches == INT_MAX) {
        std::cout << "impossible" << std::endl;
    } else {
        std::cout << maxSwitches << std::endl;
    }

}




int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t > 0) {
        testcase();
        t--;
    }
}