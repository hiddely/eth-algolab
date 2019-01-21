//
// Created by Hidde Lycklama on 1/10/19.
//

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef std::pair<bool, int> Data;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_face_base_2<K> Vb;
typedef CGAL::Triangulation_vertex_base_with_info_2<Data,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Fb, Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Edge Edge;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 Point;

bool has_interference(Triangulation const & trg, K::FT const & rr)
{
    for (auto e = trg.finite_edges_begin(); e != trg.finite_edges_end(); ++e)
        if (trg.segment(*e).squared_length() <= rr) return true;
        return false;
}

void testcase() {
    int n, m; long r;
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

    K::FT radius = K::FT(r * r);

    std::stack<Vertex_handle> toVisit;

    Triangulation t0, t1;

    int componentCount = 1;

    for (auto fiter = t.finite_vertices_begin(); fiter != t.finite_vertices_end(); fiter++) {
        fiter->info() = { false, 0 };
    }

    bool interference = false;
    for (auto fiter = t.finite_vertices_begin(); fiter != t.finite_vertices_end(); fiter++) {
        if (fiter->info().second != 0) {
            continue;
        }
        toVisit.push(fiter);
        componentCount++;
        fiter->info() = { false, componentCount };

        while (!toVisit.empty() && !interference) {
            Vertex_handle n = toVisit.top();
            toVisit.pop();

//        std::cerr << "Vertex " << n->point() << std::endl;

            Triangulation::Vertex_circulator c = t.incident_vertices(n);
            do {
                if (!t.is_infinite(c) && CGAL::squared_distance(c->point(), n->point()) <= radius) {
//            std::cerr << "V " << o->point() << " through edge " <<std::endl;
                    if (c->info().second != 0 && c->info().first == n->info().first) {
                        interference = true;
                        break;
                    }
                    if (c->info().second == 0) {
                        c->info() = { !n->info().first, componentCount };
//                std::cerr << "Assiging color " << color[o] << " to " << o->point() << std::endl;
                        toVisit.push(c);
                    }
                }
            } while (++c != t.incident_vertices(n));

            if (n->info().first) {
                t0.insert(n->point());
            } else {
                t1.insert(n->point());
            }
        }
    }

    interference = interference || has_interference(t0, radius) || has_interference(t1, radius);

    // All vertices have a color, deterministically
//    for (auto iter = t.finite_edges_begin(); iter != t.finite_edges_end(); iter++) {
//        Triangulation::Vertex_handle v1 = iter->first->vertex((iter->second + 1) % 3);
//        Triangulation::Vertex_handle v2 = iter->first->vertex((iter->second + 2) % 3);
//        if (t.segment(iter).squared_length() <= radius && color[v1] == color[v2]) {
////            std::cerr << "Adding " << t.segment(iter) << std::endl;
////            std::cerr << v1->point() << " and " << v2->point() << " are interfereing " << std::endl;
//            interference = true;
//            break;
//        }
//    }
        // read the input
    for (int i = 0; i < m; i++) {
        int ax, ay, bx, by;
        std::cin >> ax >> ay >> bx >> by;

        if (interference) {
            std::cout << "n";
            continue;
        }

        Point a(ax, ay);
        Point b(bx, by);
//        if (interference[va] || interference[vb]) {
//            std::cerr << va->point() << " " << vb->point() << " " << interference[va] << " " << interference[vb] << std::endl;
//            std::cout << "n";
//            continue;
//        }
        if (CGAL::squared_distance(a, b) <= radius) {
            std::cout << "y";
            continue;
        }


        // determine closest vertices
        Vertex_handle va = t.nearest_vertex(a);
        Vertex_handle vb = t.nearest_vertex(b);
        int sa = va->info().second;
        int sb = vb->info().second;

//        std::cerr << "Nearest " << va->point() << " " << vb->point() << std::endl;
        if (CGAL::squared_distance(a, va->point()) > radius || CGAL::squared_distance(b, vb->point()) > radius) {
            // err
//            std::cerr << "Too far!" << std::endl;
//std::cerr << (CGAL::squared_distance(a, va->point()) > radius) << std::endl;
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