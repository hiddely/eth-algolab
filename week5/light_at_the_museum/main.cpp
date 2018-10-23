//
// Created by Hidde Lycklama on 10/21/18.
//

#include <iostream>
#include <vector>
#include <tuple>
#include <climits>

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

bool match(std::vector<int> &target, std::vector<std::vector<std::pair<int, int>>> &switches, int config, int numSwitches, int numRooms) {
    for (int b = 0; b < numRooms; b++) {
        int sum = 0;
//        std::cout << "Room " << b << " ";
        for (int a = 0; a < numSwitches; a++) {
            // enabled if the bit in config is 0 / 1
            if (config & 1 << a) {
//                std::cout << "on " << switches[a][b].first << " ";
                sum += switches[a][b].first;
            } else {
//                std::cout << "off " << switches[a][b].second << " ";
                sum += switches[a][b].second;
            }
        }
//        std::cout << std::endl;
        if (sum != target[b]) {
//            std::cout << sum << " does nto equal " << target[b] << " (" << b << ")" << std::endl;
            return false;
        }
    }
    return true;
}

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

//    int split = numSwitches / 2;
//    for (int a 0 ; )

    int min = INT_MAX;
    for (int a = 1 << numSwitches; a > 0; a--) {
        std::cout << "Trying " << a << std::endl;
        if (match(target, switches, a, numSwitches, numRooms)) {
            int s = BitCount(a); // 2
            int x = numSwitches - s;
//            std::cout << "wewmatch at " << x << std::endl;
            min = std::min(min, x);
        }
    }
    if (min == INT_MAX) {
        std::cout << "impossible" << std::endl;
    } else {
        std::cout << min << std::endl;
    }
}




int main() {
    int t; std::cin >> t;
    while (t > 0) {
        testcase();
        t--;
    }
}