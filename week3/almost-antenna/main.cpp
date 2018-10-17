//
// Created by Hidde Lycklama on 10/6/18.
//

#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/Min_circle_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef K::Point_2 P;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_Circle;

void testcase(int n) {
    std::vector<P> points(n);
    for (int i = 0; i < n; i++) {
        long x; long y;
        std::cin >> x; std::cin >> y;
        points[i] = P(x, y);
    }

    Min_Circle circle(points.begin(), points.end(), true);
    Traits::Circle c = circle.circle();

    P center = c.center();
    auto minRadius = c.squared_radius();
    auto iterator = circle.support_points_begin();
    for (; iterator != circle.support_points_end(); iterator++) {
        std::vector<P> pM(points);
        for (int i = 0; i < n; i++) {
            if (pM[i] == *iterator) {
                pM.erase(pM.begin() + i);
            }
        } // Clear element
        Min_Circle mC(pM.begin(), pM.end(), true);
        Traits::Circle mCircle = mC.circle();
        minRadius = std::min(minRadius, mCircle.squared_radius());
    }
//    std::cout << c.squared_radius() << std::endl;
    std::cout << ceil(CGAL::sqrt(minRadius)) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
//    std::setprecision(0);
    while (true) {
        int n; std::cin >> n;
        if (n == 0) {
            break;
        }
        testcase(n);
    }
    return 0;
}