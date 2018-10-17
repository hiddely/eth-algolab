//
// Created by Hidde Lycklama on 10/4/18.
//

#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/intersections.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 Ray;

int iter = 0;

void testcase(int n) {
    double x; double y; double a; double b;
    std::cin >> x; std::cin >> y; std::cin >> a; std::cin >> b;

    P rayStart(x, y), rayEnd(a, b);
    Ray ray(rayStart, rayEnd);

    bool hasIntersect = false;
    for (int i = 0; i < n; i ++ ){
        double r; double s; double t; double u;
        std::cin >> r; std::cin >> s; std::cin >> t; std::cin >> u;

        if (hasIntersect) {
            continue;
        }

        P o1(r, s), o2(t, u);
        K::Segment_2 segment(o1, o2);
        if (CGAL::do_intersect(ray, segment)) {
            std::cout << "yes" << std::endl;
            hasIntersect = true;
        }
    }
    if (!hasIntersect) {
        std::cout << "no" << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    while (true) {
        iter++;
        int n; std::cin >> n;
        if (n == 0) {
            break;
        }
//        std::cout << "N " << n << " ";
        testcase(n);
    }
    return 0;
}