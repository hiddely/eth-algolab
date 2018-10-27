#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <climits>
#include <algorithm>

int size(const std::pair<int, int> &p) {
    int x = (p.second - p.first);
    if (x < 0) {
        return 10 + x;
    }
    return x;
}

bool compareEnd(const std::pair<int, int> &one, const std::pair<int, int> &two) {
//    return size(one) < size(two);
    return one.second < two.second;
}

bool compareBegin(const std::pair<int, int> &one, const std::pair<int, int> &two) {
//    return size(one) < size(two);
    return one.first < two.first;
}

int modNeg(int val, int diff, int mod) {
    int n = ((val - diff) % mod);
    if (n < 0) {
        return mod + n;
    }
    return n;
//    return (((val - diff) % mod) + mod) % mod;
}

struct E {
    int a;
    bool start;
    int originalIndex;

    E(int x, bool s, int index): a(x), start(s), originalIndex(index) {};
    E() = default;

    bool operator < (const E &other) const {
        if (this->a == other.a) {
            return this->start > other.start;
        }
        return this->a < other.a;
    }
};

int greedy(int diff, std::vector<std::pair<int, int> > &jedi, int n, int segments) {
    // Offset everything by unusedSegment
    // lay segments that go over the origin 'flat'
    std::vector<std::pair<int, int> > shift(n);
    for (int i = 0; i < n; i++) {
        int a = jedi[i].first;
        int b = jedi[i].second;
        shift[i] = std::make_pair(modNeg(a, diff, segments), modNeg(b, diff, segments));
    }

    std::sort(shift.begin(), shift.end(), compareEnd); // n log n

//    for (int i = 0; i < n; i++) {
//        std::cout << jedi[i].first << " " << jedi[i].second << std::endl;
//    }

    int count = 0;
    int coveredSegment = -1;
    for (int i = 0; i < n; i++) {
//        std::cout << "J " << jedi[i].first << " " << jedi[i].second << std::endl;
        int a = shift[i].first;
        int b = shift[i].second;
        if (a > coveredSegment) {
//            if (t == 7)
//                std::cout << "including " << a << ", " << b << std::endl;
            coveredSegment = b;
            count++;
        }
    }
    return count;
}

void testcase(int t) {
    int n, segments;
    std::cin >> n >> segments;

    std::vector<std::pair<int, int>> jedi(n);
    std::vector<E> points(n * 2);
    for (int i = 0; i < n; i++) {
        int a, b;
        std::cin >> a >> b;
        a--; b--;
        jedi[i] = std::pair<int, int>(a, b);
        points[2 * i] = E(a, true, i);
        points[2 * i + 1] = E((b + 1) % segments, false, i);
    }
//
//    if (t != 30) {
//        return;
//    }

//    std::sort(jedi.begin(), jedi.end(), compareEnd);
    std::sort(points.begin(), points.end());

    // Calculate the numbers the overlap with 0.. aka b < a
    int overlapping = 0;
    for (int i = 0; i < n; i++) {
        auto j = jedi[i];
        if (j.second < j.first) {
            overlapping++;
        } else if (j.second == segments - 1) {
            overlapping++;
        }
    }

//    std::cout << "Start " << overlapping << std::endl;

    int minOverlapping = overlapping;
    int minOverlappingIndex = 1;
    for (int i = 0; i < 2 * n; i++) {
//        std::cout << "Encountering " << points[i].a << " " << points[i].start << " " << points[i].originalIndex << std::endl;
        if (points[i].start) {
            overlapping++;
        } else {
            overlapping--;
        }
//        std::cout << "OVerlap: " << overlapping << std::endl;
        if (overlapping < minOverlapping) {
            minOverlapping = overlapping;
            minOverlappingIndex = points[i].originalIndex;
        }
    }

//    std::cout << "Num overlap: " << minOverlapping << std::endl;

    // For now, we assume its always the b of jedi[minOverlappingIndex]
    // Max 10.
    int startIndex = jedi[minOverlappingIndex].second + 1;

//    // Check
//    int check = 0;
//    for (int i = 0; i < n; i++) {
//        auto j = jedi[i];
//        if (startIndex >= j.first && startIndex <= j.second && j.first < j.second) {
//            check++;
//        } else if (j.second < j.first && (startIndex > j.first || startIndex < j.second)) {
//            check++;
//        }
//    }
//    std::cout << "Check: " << (check == minOverlapping) << " " << check << " == " << minOverlapping << std::endl;
//    if (check != minOverlapping) {
//        std::cout << "Original: " << jedi[minOverlappingIndex].first << " " << jedi[minOverlappingIndex].second << std::endl;
//    }

    // Offset everything by unusedSegment
    // lay segments that go over the origin 'flat'
    std::vector<std::pair<int, int> > shift;
    std::vector<std::pair<int, int> > potentialStarts;
    for (int i = 0; i < n; i++) {
        int a = jedi[i].first;
        int b = jedi[i].second;
        std::pair<int, int> shifted = std::make_pair(modNeg(a, startIndex, segments), modNeg(b, startIndex, segments));
//        std::cout << "Shifted " << shifted.second << " < " << shifted.first << std::endl;
         if (shifted.second < shifted.first) {
             // overlapping
             potentialStarts.push_back(shifted);
         } else if (shifted.first == 0) {
             potentialStarts.push_back(shifted);
        } else {
            shift.push_back(shifted);
        }
    }

    if (minOverlapping != potentialStarts.size()) {
        std::cout << "ERROR ! ";
        return;
    }

    std::sort(shift.begin(), shift.end(), compareEnd); // n log n

    int countMin = INT_MIN;
    if (minOverlapping > 0) {
        for (int s = 0; s < minOverlapping; s++) {
            // Start at every potentialStart
            int count = 1;
            int coveredSegment = potentialStarts[s].second;
            int maxB = potentialStarts[s].first;
//            std::cout << "S " << maxB << " " << coveredSegment << std::endl;
            for (int i = 0; i < n - minOverlapping; i++) {
                int a = shift[i].first;
                int b = shift[i].second;
//                std::cout << "O " << shift[i].first << " " << shift[i].second << std::endl;
                if (a > coveredSegment && (b < maxB || maxB == 0)) {
//                    std::cout << "J " << shift[i].first << " " << shift[i].second << std::endl;
                    coveredSegment = b;
                    count++;
                }
            }
            countMin = std::max(countMin, count);
        }
    }

    // The 0 option, dont pick any
        int count = 0;
        int coveredSegment = -1;
        for (int i = 0; i < n - minOverlapping; i++) {
//        std::cout << "J " << shift[i].first << " " << shift[i].second << std::endl;
            int a = shift[i].first;
            int b = shift[i].second;
            if (a > coveredSegment) {
                coveredSegment = b;
                count++;
            }
        }
        countMin = std::max(countMin, count);

//    std::cout << minOverlapping << " ";
    std::cout << countMin << std::endl;
}



int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t > 0) {
        testcase(t);
        t--;
    }
}