//
// Created by Hidde Lycklama on 9/24/18.
//

#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>

struct Occ {
    int word;
    int pos;

    Occ(int w, int p) : word(w), pos(p) {}
    Occ() = default;

    bool operator < (const Occ& str) const
    {
        return (pos < str.pos);
    }
};


void testcase() {
    int words; std::cin >> words;
    std::vector<int> occurs(words);
    int totalItems = 0;
    for (int i = 0; i < words; i++) {
        std::cin >> occurs[i];
        totalItems += occurs[i];
    }
    std::vector<Occ> items(totalItems);
    int currentIndex = 0;
    for (int i = 0; i < words; i++) {
        for (int a = 0; a < occurs[i]; a++) {
            int pos; std::cin >> pos;
            Occ item = Occ(i, pos);
            items[currentIndex] = item;
            currentIndex++;
        }
    }
    std::sort(items.begin(), items.end());
    int interval = 0;
    int minPos = -1;
//    int lowestIndex = 0;
//    std::list<Occ*> window;
    int minInterval = std::numeric_limits<int>::max();
    std::vector<int> currentIndexOfWord(words, -1);
    std::list<int> positions;
    for (int i = 0; i < totalItems; i++) {
        Occ item = items[i];

        if (currentIndexOfWord[item.word] == minPos) {
            // We're replacing the min
            currentIndexOfWord[item.word] = item.pos;
            minPos = *std::min_element(currentIndexOfWord.begin(), currentIndexOfWord.end());
        } else {
            currentIndexOfWord[item.word] = item.pos;
        }

        auto max = item.pos;

        if (minPos == -1) {
            continue; // not all words are in the interval yet
        }

        interval = max - minPos + 1;
        if (interval < minInterval) {
            minInterval = interval;
        }
    }

    std::cout << minInterval << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
