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
//            std::cout << p << std::endl;
            pts[i] = p;
        }
        // construct triangulation
        Triangulation t;
        t.insert(pts.begin(), pts.end());
        // output all edges
        // Find smallest edge between two points
//        std::cout << "edg " << std::endl;
        double min = std::numeric_limits<double>::max();
        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        {
            K::FT sl = t.segment(e).squared_length();
            if (sl < min) {
                min = sl;
            }
        }
        std::cout << root(min) << std::endl;
//            std::cout << t.segment(e) << " " << t.segment(e).squared_length() << "\n";
    }
}
