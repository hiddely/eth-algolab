//
// Created by Hidde Lycklama on 11/10/18.
//

#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <limits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

double root(const K::FT& x) {
    double s = std::sqrt(CGAL::to_double(x)) * 50;
    return std::ceil(s);
}

int main()
{
    while (true) {
        // read number of points
        std::size_t n;
        std::cin >> n;
        if (n == 0) {
            return 0;
        }
        // read points
        std::vector<K::Point_2> pts (n);
        for (std::size_t i = 0; i < n; ++i) {
            int x, y;
            std::cin >> x >> y;
            K::Point_2 p(x, y);
            pts[i] = p;
        }
        // construct triangulation
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        std::size_t m;
        std::cin >> m;
        for (std::size_t i = 0; i < m; i++) {
            K::Point_2 p;
            std::cin >> p;
            std::cout << std::fixed << std::setprecision(0) << CGAL::squared_distance(p, t.nearest_vertex(p)->point()) << std::endl;
        }
    }
}
