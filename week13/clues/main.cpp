//
// Created by Hidde Lycklama on 1/10/19.
//

#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Edge Edge;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 Point;

void testcase() {
    int n, m, r;
    std::cin >> n >> m >> r;

    std::vector<Point> pts(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        pts[i] = Point(x, y);
    }

    Triangulation t;
    t.insert(pts.begin(), pts.end());

    K::FT maxDist = K::FT(4 * r * r);

    // Construct the MST
    std::map<Vertex_handle, int> rank_map;
    std::map<Vertex_handle, Vertex_handle> parent_map;
    boost::associative_property_map< std::map<Vertex_handle, int> >
            rank(rank_map);
    boost::associative_property_map< std::map<Vertex_handle, Vertex_handle> >
            parent(parent_map);

    boost::disjoint_sets<boost::associative_property_map< std::map<Vertex_handle, int> >,
            boost::associative_property_map< std::map<Vertex_handle, Vertex_handle> > > dset(rank, parent);

    for (auto iter = t.finite_vertices_begin(); iter != t.finite_vertices_end(); iter++) {
        // check that its neighbors are sufficiently far from each other
        dset.make_set(iter);
    }

    K::FT radius = K::FT(r * r);
    std::vector<Edge> edges;
    for (auto iter = t.finite_edges_begin(); iter != t.finite_edges_end(); iter++) {
        if (t.segment(iter).squared_length() <= radius) {
//            std::cerr << "Adding " << t.segment(iter) << std::endl;
            edges.push_back(*iter);
        }
    }
    std::sort(edges.begin(), edges.end(), [t](const Edge &e1, const Edge &e2) {
        return t.segment(e1).squared_length() < t.segment(e2).squared_length();
    });

    std::map<Edge, bool> interference;
    for (auto iter = t.finite_vertices_begin(); iter != t.finite_vertices_end(); iter++) {
        // check that its neighbors are sufficiently far from each other
        Triangulation::Edge_circulator c = t.incident_edges(iter);
        Point lastPoint;
        bool lInit = false;
        do {
            if (!t.is_infinite(c)) {
                if (lInit) {
                    Point thisPoint = t.segment(c).target();
                    K::FT dist = CGAL::squared_distance(lastPoint, thisPoint);
//                    std::cerr << "Comparing " << lastPoint << " " << thisPoint << " " << dist << std::endl;
                    if (dist < radius) {
//                        std::cout << component->point() << " has interference! " <<std::endl;
                        interference[*c] = true;
                        interference[*(c--)] = true;
                        c++;
//                        break;
                    }
                    lastPoint = thisPoint;
                } else {
                    lInit = true;
                    lastPoint = t.segment(c).target();
                }
            }
        } while (++c != t.incident_edges(iter));
    }

    for (auto iter = edges.begin(); iter != edges.end(); iter++) {
        if (interference[*iter]) {
            continue;
        }
        Vertex_handle u = iter->first->vertex((iter->second + 1) % 3);
        Vertex_handle v = iter->first->vertex((iter->second + 2) % 3);
        Vertex_handle su = dset.find_set(u);
        Vertex_handle sv = dset.find_set(v);
        if (su != sv) {
//            std::cerr << u->point() << " " << v->point() << ", Uniting " << su->point() << " " << sv->point() << std::endl;
            dset.union_set(su, sv);
        }
    }

    bool success = true;
    Vertex_handle total;
    bool inittotal = false;
    for (auto iter = t.finite_vertices_begin(); iter != t.finite_vertices_end(); iter++) {
        if (!inittotal) {
            total = dset.find_set((Vertex_handle)iter);
            inittotal = true;
        } else {
            Vertex_handle n = dset.find_set((Vertex_handle)iter);
            if (!(total == n)) {
                success = false;
            }
        }
    }

        // read the input
    for (int i = 0; i < m; i++) {
        int ax, ay, bx, by;
        std::cin >> ax >> ay >> bx >> by;
        Point a(ax, ay);
        Point b(bx, by);

        if (!success) {
            std::cout << "n";
            continue;
        }

        // determine closest vertices
        Vertex_handle va = t.nearest_vertex(a);
        Vertex_handle vb = t.nearest_vertex(b);
        Vertex_handle sa = dset.find_set(va);
        Vertex_handle sb = dset.find_set(vb);
//        if (interference[va] || interference[vb]) {
//            std::cerr << va->point() << " " << vb->point() << " " << interference[va] << " " << interference[vb] << std::endl;
//            std::cout << "n";
//            continue;
//        }
        if (CGAL::squared_distance(a, b) <= radius) {
            std::cout << "y";
            continue;
        }
//        std::cerr << "Nearest " << va->point() << " " << vb->point() << std::endl;
        if (CGAL::squared_distance(a, va->point()) > radius || CGAL::squared_distance(b, vb->point()) > radius) {
            // err
//            std::cerr << "Too far!" << std::endl;
            std::cout << "n";
            continue;
        }
        if (sa == sb) {
//            std::cerr << sa->point() << " == " << sb->point() << std::endl;
            std::cout << "y";
        } else {
//            std::cerr << sa->point() << " != " << sb->point() << std::endl;
            std::cout << "n";
        }
    }


    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}