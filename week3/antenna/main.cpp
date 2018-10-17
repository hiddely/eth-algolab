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
    P points[n];
    for (int i = 0; i < n; i++) {
        long x; long y;
        std::cin >> x; std::cin >> y;
        points[i] = P(x, y);
    }

    Min_Circle circle(points, points+n, true);
    Traits::Circle c = circle.circle();
//    std::cout << c.squared_radius() << std::endl;
    std::cout << ceil(CGAL::sqrt(c.squared_radius())) << std::endl;
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