//
// Created by Hidde Lycklama on 1/2/19.
//

#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <limits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 Point;

bool too_small(long edge_squared, int t) {
    return 4 * std::pow(t, 4) + 4 * std::pow(t, 2) + 1 < edge_squared;
}

void binary_search(long val) {

    int lmin = 0, lmax = 1;
    while (too_small(val, lmax)) lmax *= 2;

    while (lmin != lmax) {
        int p = (lmin + lmax)/2;
        if (too_small(val, p))
            lmin = p + 1;
        else
            lmax = p;
    }
    int L = lmin;
    std::cout << L;
}

void testcase(int n) {
    int il, ib, ir, it;
    std::cin >> il >> ib >> ir >> it;
    K::FT l = K::FT(il);
    K::FT b = K::FT(ib);
    K::FT r = K::FT(ir);
    K::FT t = K::FT(it);

    std::vector<Point> pts(n);
    for (int i = 0; i < n; i++) {
        Point p;
        std::cin >> p;
        pts[i] = p;
    }

    Triangulation tr;
    tr.insert(pts.begin(), pts.end());

    std::vector<K::FT> edges;

    for (Vertex_iterator iterator = tr.finite_vertices_begin(); iterator != tr.finite_vertices_end(); iterator++) {
        Vertex_handle v = iterator;
        Triangulation::Edge_circulator c = tr.incident_edges(v);
        K::FT minEdgeSize = std::numeric_limits<double>::max();
//            if (c != tr.incident_edges(v)) {
        do {
            if (!tr.is_infinite(c)) {
//                    std::cout << tr.segment(c) << " " << tr.segment(c).squared_length() << std::endl;
                minEdgeSize = std::min(minEdgeSize, tr.segment(c).squared_length());
            }
        } while (++c != tr.incident_edges(v));
//            }

//            std::cout << "Pushing " << minEdgeSize << std::endl;
        // Check with edges...
        Point p = v->point();
        int dL = CGAL::to_double(p.x()) - il;
        int dR = ir - CGAL::to_double(p.x());
        int dT = it - CGAL::to_double(p.y());
        int dB = CGAL::to_double(p.y()) - ib;
        int dist = std::min(dL, std::min(dR, std::min(dT, dB)));
        minEdgeSize = std::min(minEdgeSize, K::FT(pow(2 * dist, 2)));
        edges.push_back(minEdgeSize);
    }

//    for (int i = 0; i < edges.size(); i++) {
//        std::cerr << edges[i] << std::endl;
//    }

//    for (Edge_iterator e = tr.finite_edges_begin(); e != tr.finite_edges_end(); ++e) {
//        edges.push_back(tr.segment(e).squared_length());
////        std::cout << tr.segment(e) << "\n";
//    }
    std::sort(edges.begin(), edges.end());

    // more than 50%
    int moreThan50 = n / 2; // 0-based


    if (edges.size() > 0) {
        binary_search(CGAL::to_double(edges[0])); std::cout << " ";
        binary_search(CGAL::to_double(edges[moreThan50])); std::cout << " ";
        binary_search(CGAL::to_double(edges[n - 1]));
        std::cout << std::endl;
    }
}

int main() {
    while(true) {
        int n; std::cin >> n;
//        std::cout << n << std::endl;
        if (n == 0) {
            break;
        }
        testcase(n);
    }
    return 0;
}