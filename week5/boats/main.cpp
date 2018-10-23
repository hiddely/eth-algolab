#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <climits>

struct Boat {
    int l;
    int p;

    Boat(int il, int ip): l(il), p(ip) {}

    bool operator < (const Boat &b) const {
        return this->p < b.p;
    }

    int minA() const {
        return p - l;
    }
    int maxB() const {
        return p + l;
    }
};

//struct Interval {
//    int a;
//    int b;
//    int boatNum;
//
//    Interval(int xa, int xb, int bn): a(xa), b(xb), boatNum(bn) {}
//
//    bool operator < (const Interval &i) const {
//        return this->b < i.b;
//    }
//};

//void findSuitabl

void testcase() {
    int n; std::cin >> n;

//    std::list<Interval> intervals;
    std::list<Boat> boats;
    for (int i = 0; i < n; i++) {
        int l, p;
        std::cin >> l >> p;
        boats.push_back(Boat(l, p));
    }

    boats.sort(); // n log n


    int finishTime = INT_MIN;
    int count = 0;
    auto it = boats.cbegin();
    int prev = finishTime;
    for (; it != boats.cend(); it++) {
        if (it->p >= finishTime) {
            // we can lay out it somewhere
            // Either mostly to the left or next to the previous boat
            prev = finishTime;
            finishTime = std::max(it->p, finishTime + it->l);
            count++;
        } else {
            int n = std::max(it->p, prev + it->l);
            if (n < finishTime) {
                finishTime = n;
            }
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