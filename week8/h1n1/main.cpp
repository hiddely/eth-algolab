//
// Created by Hidde Lycklama on 11/10/18.
//

#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <stack>
#include <vector>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Face_handle Face_handle;
typedef K::Point_2 Point;

struct DoubleDefaultedToMinusOne
{
    double i = -1;

    DoubleDefaultedToMinusOne(double a): i(a) {}
};

int main() {
    while (true) {
        int n; std::cin >> n;
        if (n == 0) {
            return 0;
        }

        std::vector<Point> ps(n);
        for (int i = 0; i < n; i++) {
            std::cin >> ps[i];
        }

        Triangulation t;
        t.insert(ps.begin(), ps.end());


        // We try for every face to compute the max distance
        for (Face_iterator start = t.finite_faces_begin(); start != t.finite_faces_end(); ++start) {

            std::stack<Face_handle> toVisit;
            std::map<Face_handle, bool> visited;
            toVisit.add(*start);
            visited[*start] = true;
            std::map<Face_handle, DoubleDefaultedToMinusOne> maxDistance;
            maxDistance[*start] = std::numeric_limits<double>::max();

            while(!toVisit.empty()) {
                Face_handle f = toVisit.front();
                f.pop();

                if (t.is_infinite(f)) {

                }

                for (int i = 0; i < 3; i++) {
                    auto v1 = f->vertex(i % 3);
                    auto v2 = f->vertex((i + 1) % 3);
                    int neigh = (i + 2) % 3;
                    Face_handle face = f->neighbor(neigh);
                    double dist = CGAL::squared_distance(v1->point(), v2->point());
                    double curMaxDist = maxDistance[f];
                    double thisPathDistance = min(curMaxDist, dist);
                    maxDistance[face] = max(thisPathDistance, maxDistance[face]);
                    if (visited[face]) {
                        continue;
                    }
                    visited[face] = true;
                    toVisit.push(face);
                }
            }
        }

        int m; std::cin >> m;
        for (int i = 0; i < m; i++) {
            Point person;
            std::cin >> person;
            long d; std::cin >> d;
            // Probably inefficient
            Face_handle start = t.locate(person);
//            std::cout << "Person " << person << std::endl;

            std::map<Face_handle, bool> visited;
            std::stack<Face_handle> toVisit;

            Point nearestInfected = t.nearest_vertex(person)->point();
            double d1 = CGAL::squared_distance(person, nearestInfected);
            if (d1 >= d) {
                // if we are already to close, ignore
                toVisit.push(start);
                visited[start] = true;
            }

            bool reached = false;

            while (!reached && !toVisit.empty()) {
                Face_handle f = toVisit.top();
                toVisit.pop();


//                std::cout << "Visiting " << t.dual(f);
//                std::cout << " made up by " << f->vertex(0)->point() << ", " << f->vertex(1)->point() << ", " << f->vertex(2)->point();
//                std::cout << std::endl;

                if (t.is_infinite(f)) {
                    // we have reached an exit
                    reached = true;
                    break;
                }

                for (int i = 0; i < 3; i++){
                    auto v1 = f->vertex(i % 3);
                    auto v2 = f->vertex((i + 1) % 3);
                    int neigh = (i + 2) % 3;
                    Face_handle face = f->neighbor(neigh);
                    if (visited[face]) {
                        continue;
                    }
                    double dist = CGAL::squared_distance(v1->point(), v2->point());
                    if (dist >= d*4.0) {
//                        std::cout << "Can pass through " << dist << " cause " << d << std::endl;
//                        std::cout << "Dist " << v1->point() << " " << v2->point() << " " << dist << " " << d << std::endl;
                        // we can pass
                        visited[face] = true;
                        toVisit.push(face);
                    }
                }
//                for (int i = 0; i < 3; i++) {
//                    Face_handle n = f->neighbor(i);
//                    if (visited[n]) {
//                        continue;
//                    }
//                    Point c = t.dual(n);
//                    Point v1 = n->vertex(0)->point();
//                    std::cout << "Dist " << c << " " << v1 << " " << d << std::endl;
//                    if (CGAL::squared_distance(c, v1) >= d) {
//                        toVisit.push(n);
//                    }
//                }
            }
            if (reached) {
                std::cout << "y";
            } else {
                std::cout << "n";
            }
        }
        std::cout << std::endl;
    }
}