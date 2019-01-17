//
// Created by Hidde Lycklama on 1/17/19.
//

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <climits>
#include <set>
#include <tuple>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
//typedef K::Triangle_2 T;
typedef K::Line_2 L;
typedef std::tuple<P, P, P, P, P, P> Tri;

P getPoint() {
    int x, y; std::cin >> x >> y;
    return P(x, y);
}

Tri createTriangle() {
    P p1 = getPoint();
    P p2 = getPoint();
    P p3 = getPoint();
    P p4 = getPoint();
    P p5 = getPoint();
    P p6 = getPoint();

    if (!CGAL::left_turn(p1, p2, p3)) { // Important triangle learnings!!!
        std::swap(p1, p2);
    }
    if (!CGAL::left_turn(p3, p4, p5)) {
        std::swap(p3, p4);
    }
    if (!CGAL::left_turn(p5, p6, p1)) {
        std::swap(p5, p6);
    }
    return Tri(p1, p2, p3, p4, p5, p6);
//    std::cerr << "Invlaid triangle" <<std::endl;
//    abort();
}

bool intersects(Tri &tri, S &s) {
    P p1 = std::get<0>(tri);
    P p2 = std::get<1>(tri);
    P p3 = std::get<2>(tri);
    P p4 = std::get<3>(tri);
    P p5 = std::get<4>(tri);
    P p6 = std::get<5>(tri);

//    std::cerr << !CGAL::right_turn(p1, p2, s.source()) << " " << !CGAL::right_turn(p3, p4, s.source()) << " " << !CGAL::right_turn(p5, p6, s.source())  << " " <<
//    !CGAL::right_turn(p1, p2, s.target())  << " " << !CGAL::right_turn(p3, p4, s.target())  << " " << !CGAL::right_turn(p5, p6, s.target()) << std::endl;

    return !CGAL::right_turn(p1, p2, s.source()) && !CGAL::right_turn(p3, p4, s.source()) && !CGAL::right_turn(p5, p6, s.source()) &&
        !CGAL::right_turn(p1, p2, s.target()) && !CGAL::right_turn(p3, p4, s.target()) && !CGAL::right_turn(p5, p6, s.target());
}

void testcase() {
    int m, n;
    std::cin >> m >> n;

    std::vector<S> path(m - 1);
    P prev; std::cin >> prev;
    for (int i = 1; i < m; i++) {
        P cur; std::cin >> cur;
        path[i - 1] = S(prev, cur);
        prev = cur;
    }

    std::vector<Tri> parts(n);
    for (int i = 0; i < n; i++) {
        parts[i] = createTriangle();
//        std::cerr << parts[i] << std::endl;
    }

//    std::cerr << "precover " << std::endl;

    // pre compute who covers what
    std::vector<std::vector<int>> covers(n); // Map part -> path
    for (int i = 0; i < n; i++) {
        for (int a = 0; a < m-1; a++) {
            if (intersects(parts[i], path[a])) {
//                std::cerr << i << ": intersects with " << path[a] << std::endl;
                covers[i].push_back(a);
            }
        }
    }

//    std::cerr << "precheck " << std::endl;

//    std::vector<bool> covered
    int upper = 0; int lower = 0;
    int minCost = INT_MAX;

    std::vector<bool> covered(m-1, false);
    std::vector<bool> coveredCheck(m-1, true);
    std::vector<std::set<int>> coveredBy(m - 1);

    while (upper <= n) {

//        std::cerr << lower << "-" << upper << std::endl;
        if (covered == coveredCheck) {
            // increase lower
            // record
            minCost = std::min(minCost, upper - lower);
            lower++;

            for (auto iter = covers[lower - 1].begin(); iter != covers[lower - 1].end(); iter++) {
                coveredBy[*iter].erase(lower - 1);
                if (coveredBy[*iter].empty()) {
                    covered[*iter] = false;
                }
            }
        } else if (upper == n) {
            break;
        } else {
            // increase upper
            upper++;
            for (auto iter = covers[upper - 1].begin(); iter != covers[upper - 1].end(); iter++) {
                coveredBy[*iter].insert(upper - 1);
                covered[*iter] = true;
            }
        }

    }

    std::cout << minCost << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}