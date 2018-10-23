#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <climits>

int size(const std::pair<int, int> &p) {
    int x = (p.second - p.first);
    if (x < 0) {
        return 10 + x;
    }
    return x;
}

bool compare(const std::pair<int, int> &one, const std::pair<int, int> &two) {
//    return size(one) < size(two);
    return one.second < two.second;
}

void testcase() {
    int n, segments; std::cin >> n >> segments;

    std::vector<std::pair<int, int>> jedi(n);
    int unusedSegment = -1;
    for (int i = 0; i < n; i++) {
        int a, b;
        std::cin >> a >> b;
        jedi[i] = std::pair<int,int>(a,b);
        if (unusedSegment == -1 && i > 0) {
            if (jedi[i].first > jedi[i - 1].second + 1) {
                unusedSegment = jedi[i - 1].second + 1;
            }
        }
    }

    // Offset everything by unusedSegment
    // lay segments that go over the origin 'flat'
    for (int i = 0; i < n; i++) {
        int a = jedi[i].first;
        int b = jedi[i].second;
        if (b < a) {
            jedi[i].second = a + segments;
        }
    }

    std::sort(jedi.begin(), jedi.end(), compare); // n log n

    int count = 0;
    int coveredSegment = 0;
    for (int i = 0; i < n; i++) {
        std::cout << "J " << jedi[i].first << " " << jedi[i].second << std::endl;
        int a = jedi[i].first;
        int b = jedi[i].second;
        if ()
        if (b > coveredSegment) {
            coveredSegment = b;
            count++;
        }
    }
    std::cout << count << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t > 0) {
        testcase();
        t--;
    }
}