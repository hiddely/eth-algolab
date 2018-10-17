//
// Created by Hidde Lycklama on 10/10/18.
//

#include <iostream>
#include <vector>

//int maxNum(int attackers, int str, int a, int b, int sum, int n, std::vector<int> &defense) {
//    if (b > a) {
//        return 0;
//    }
//    if (at >= n) {
//        return 0;
//    }
//    if (sum == str) {
//        return b - a + 1 + maxNum(attackers--);
//    }
//    int incA = maxNum(attackers)
//}

struct Option {
    int a;
    int b;

    Option(int aa, int bb): a(aa), b(bb) {};
    Option() = default;

    bool overlaps(Option &next) {
        return this->b >= next.a;
    }

    int distance() {
        return this->b - this->a + 1;
    }
};

std::vector<std::vector<int>> lookup;

int selectMax(int i, int n, int attackers, std::vector<Option> &options) {
    if (i >= n) {
        return 0;
    }
    if (attackers == 0) {
        return 0; // 0 left
    }
    Option &cur = options[i];
    if (i == n - 1) {
        return cur.distance();
    }
    if (lookup[i][attackers] != -1) {
        return lookup[i][attackers];
    }
    int max = 0; // choosing this option
    int nonOverlappingIndex = n;
    for (int a = i + 1; a < n; a++ ){
        Option &other = options[a];
        if (cur.overlaps(other)) {
//            std::cout << " i " << i << " overlaps " << a << std::endl;
//            max = std::max(selectMax(a, n, attackers, options), max); // option of choosing the other one
        } else {
//            std::cout << "nonOverlap " << a << std::endl;
            nonOverlappingIndex = a;
            break;
        }
    }
    // choosing, this skipping the rest
    int choosethis = cur.distance() + selectMax(nonOverlappingIndex, n, attackers - 1, options);
    int skipthis = selectMax(i + 1, n, attackers, options);
    int choice = std::max(std::max(choosethis, max), skipthis);

//    std::cout << "i " << i << " MAx " << max << " choose " << choosethis << " index " << nonOverlappingIndex << std::endl;

    lookup[i][attackers] = choice;

    return choice;
}


void testcase() {
    int numDefense; std::cin >> numDefense;
    int numAttack; std::cin >> numAttack;
    int attackStrength; std::cin >> attackStrength;
    std::vector<int> defenseValues(numDefense);
    for (int i = 0; i < numDefense; i++) {
        std::cin >> defenseValues[i];
    }


    int a = 0; int b = 0; int sum = 0;
    std::vector<Option> potentialPositions;
    while (a < numDefense && (b < numDefense || sum > attackStrength)) {
        if (b < numDefense && sum <= attackStrength) {
            sum += defenseValues[b];
            b++;
//            std::cout << "Increasing B " << b << ", sum: " << sum << std::endl;
        }
        if (sum > attackStrength) {
            // Too strong
            sum -= defenseValues[a];
            a++;
//            std::cout << "Increasing A " << a << ", sum: " << sum << std::endl;
        }
        if (sum == attackStrength) {
            // We have found one
//            std::cout << "Found one " << a << " " << b - 1 << std::endl;
            potentialPositions.push_back(Option(a, b - 1));
        }
    }

//    for


    int n = potentialPositions.size();

//    for(int i = 0; i < n; i++) {
//        std::cout << potentialPositions[i].a << " " << potentialPositions[i].b << std::endl;
//    }

    if (n < numAttack) {
        std::cout << "fail" << std::endl;
        return;
    }
//    std::cout << n << std::endl;
    lookup = std::vector<std::vector<int>>(n, std::vector<int>(numAttack + 1, -1));

    int kewl = selectMax(0, n, numAttack, potentialPositions);

    bool couldPlaceAllAttackers = false;
    for (int i = 0; i < n; i++) {
        if (lookup[i][1] != -1) {
            couldPlaceAllAttackers = true;
        }
    }

    if (!couldPlaceAllAttackers) {
        std::cout << "fail" << std::endl;
        return;
    }

//    for (int i = 0; i < n; i++) {
//        for (int a = 0; a < numAttack + 1; a++) {
//            std::cout << lookup[i][a] << " ";
//        }
//        std::cout << std::endl;
//    }
//    std::cout << std::endl;

    std::cout << kewl << std::endl;
}

int main() {
    int t; std::cin >> t;
    for (int a = 0; a < t; a++) {
        testcase();
    }
    return 0;
}