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

    bool operator ==(const Sum &o) const {
        return this->s == o.s;
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

    int split1 = numSwitches / 2;
    int split2 = numSwitches - split1;
    int count1 = 1 << split1;
    int count2 = 1 << split2;

    std::vector<std::vector<int> > sum1(count1, std::vector<int>(numRooms));
    std::vector<std::vector<Sum> > sum2(count2, std::vector<Sum>(numRooms));
    for (int b = 0; b < numRooms; b++){
        for (int a = count1 - 1; a >= 0; a--) {
            int s = sum(b, switches, a, 0, split1);
//            std::bitset<16> on(a);
//            std::cout << a << ": s(" << s << ") " << on << std::endl;
//            std::cout << split1 - BitCount(a) << ": " << s << std::endl;
            sum1[a][b] = s;
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

            sum2[a][b] = Sum(s, a);
        }
    }

    std::sort(sum2.begin(), sum2.end());

//    std::cout << "Sum 2:" << std::endl;
//    for (int a = 0; a < count2; a++) {
//        std::cout << sum2[a][0].index << ": ";
//        for (int b = 0; b < numRooms; b++) {
//            std::cout << sum2[a][b].s << " ";
//        }
//        std::cout << std::endl;
//    }

    int min = INT_MAX;
    for (int a = 0; a < count1; a++) {
        std::vector<Sum> s(numRooms);
//        std::cout << "looking for ";
        for (int b = 0; b < numRooms; b++) {
            s[b] = Sum(target[b] - sum1[a][b], 0);
//            std::cout << s[b].s << " ";
        }
//        std::cout << std::endl;

        int bc1 = split1 - BitCount(a);

        auto iter = std::lower_bound(sum2.begin(), sum2.end(), s);
        while (iter != sum2.end() && *iter == s) { // get all values
//            std::cout << "Found match " << a << ", " << ((*iter)[0]).index << std::endl;
            int bc2 = split2 - BitCount((*iter)[0].index);
            if (bc1 + bc2 < min) {
                min = bc1 + bc2;
            }
            iter++;
        }
    }
    if (min == INT_MAX) {
        std::cout << "impossible" << std::endl;
    } else {
        std::cout << min << std::endl;
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

//    std::set<int> topMatches;
//    for (int a = 0; a < count1 - 1; a++) { // the choice
//        // Potential for saving: Keep track which indexes
//        std::vector<std::set<int>> matches(numRooms);
//        for (int b = 0; b < numRooms; b++ ) {
//            int s1 = sum1[b][a];
//            Sum diff = Sum(target[b] - s1, -1);
//            auto elem = sum2[b].lower_bound(diff);
//            if (elem->s == diff.s) {
//                matches[b].insert(elem->index);
//                std::cout << "Match: " << a << "," << elem->index << std::endl;
////                elem++;
//            }
////                    std::cout << b << " Match " << elem->index << ": " << s1 << " + " << elem->s << " = " << target[b]
////                              << std::endl;
////                match[b] = true;
//
//
//        }
//        std::set<int> m = matches[0];
//
////        for (int b = 1; b < numRooms; b++) {
//////            auto r = matches[b].begin();
//////            for (; r != matches[b].end(); r++) {
//////                std::cout << "Testing " << *r << std::endl;
//////            }
////            std::set<int>temp;
////            set_intersection(m.begin(), m.end(), matches[b].begin(), matches[b].end(), std::inserter(temp,temp.begin()));
//////            auto t = temp.begin();
//////            for (; t != temp.end(); t++) {
//////                std::cout << "M Contains " << *t << std::endl;
//////            }
////            m = temp;
////        }
//
//        int g = split1 - BitCount(a);
//        auto iter = m.begin();
//        int min = INT_MAX;
//        for (; iter != m.end(); iter++) {
//            int h = split2 - BitCount(*iter);
////            std::bitset<16> b(*iter);
////            std::bitset<16> c(g);
////            std::cout << "Bits: " << h << " " << b << " " << g << " " << c << std::endl;
//            min = std::min(g + h, min);
//        }
//        if (min < INT_MAX) {
//            topMatches.insert(min);
//        }
//    }
}




int main() {
    int t; std::cin >> t;
    while (t > 0) {
        testcase();
        t--;
    }
}