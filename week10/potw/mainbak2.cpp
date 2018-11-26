//
// Created by Hidde Lycklama on 11/19/18.
//

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 Point;

void testcase() {
    int n, m; long p;
    std::cin >> n >> m >> p;

    std::vector<Point> jammers(n);
    for (int i = 0; i < n; i++) {
        std::cin >> jammers[i]; // should work
    }

    Triangulation t;
    t.insert(jammers.begin(), jammers.end()); // construct triangulation for NN Graph

    // Calculate the min-reachable
//    for (Triangulation::Vertex_handle v = t.ed)

    // Naive:
    double minRequiredDistance = 0;
    for (int i = 0; i < m; i++) {
        Point pointS, pointT;
        std::cin >> pointS >> pointT;
        // check which vertex both belong to
        Triangulation::Vertex_handle start = t.nearest_vertex(pointS);
        Triangulation::Vertex_handle end = t.nearest_vertex(pointT);

        std::priority_queue<Vertex_handle> toVisit;
        std::map<Vertex_handle, bool> visited;
        std::map<Vertex_handle, double> distances;
        double dStart = CGAL::squared_distance(start->point(), pointS) * 4.0;
        double dEnd = CGAL::squared_distance(end->point(), pointT) * 4.0;
        if (dStart <= p &&
            dEnd <= p) {
            // The initial point is inside the range of a jammer...
            // AND the finish point
            toVisit.push(start);
            visited[start] = true;
            distances[start] = 0;
        }
        minRequiredDistance = std::max(minRequiredDistance, dStart);
        minRequiredDistance = std::max(minRequiredDistance, dEnd);
        bool found = false;
        while (!toVisit.empty() && !found) {
            auto V = toVisit.top();
            toVisit.pop();

            if (V == end) {
                found = true;
                break;
            }
//            std::cout << "Visiting " << V->point() << " ";

            Triangulation::Edge_circulator c = t.incident_edges(V);
            do {
                Triangulation::Vertex_handle v1 = c->first->vertex((c->second + 1) % 3);
                Triangulation::Vertex_handle v2 = c->first->vertex((c->second + 2) % 3);
//                std::cout << "Found edge " << v1->point() << " -> " << v2->point() << " " << !t.is_infinite(c) << std::endl;
                if (!t.is_infinite(c)) {
                    double dist = t.segment(*c).squared_length();
//                    if (distances[v1] != 0) {
//                        dist = std::min(distances[v1], dist);
//                    }
                    distances[v1] = std::max(distances[v2], dist);
                    if (!visited[v1]) {
//                        std::cout << "Pushing " << v1->point() << " " << t.segment(*c).squared_length() << std::endl;
                        visited[v1] = true;
                        toVisit.push(v1);
                    }
                }
            } while (++c != t.incident_edges(V));
        }
        if (found && distances[end] <= p) {
            // Found
            std::cout << "y";
        } else {
            std::cout << "n";
        }
        minRequiredDistance = std::max(minRequiredDistance, distances[end]);
    }
    std::cout << std::endl;
    std::cout << std::setprecision(0) << std::fixed << minRequiredDistance << std::endl;
}

int main() {
    int t; std::cin >> t;
    while(t--) {
        testcase();
    }
    return 0;
}