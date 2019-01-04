//
// Created by Hidde Lycklama on 11/10/18.
//

#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <stack>
#include <vector>
#include <map>
#include <queue>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Face_handle Face_handle;
typedef K::Point_2 Point;

struct DoubleDefaultedToMinusOne
{
    double i = -1;

    DoubleDefaultedToMinusOne(double a): i(a) {}
};

struct Entry {
    Face_handle fh;
    K::FT size;

    Entry(Face_handle a, K::FT b): fh(a), size(b) {}

    bool operator < (const Entry &other) const {
        return this->size < other.size; // biggest first
    }
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

        std::priority_queue<Entry> toVisit;
        std::map<Face_handle, bool> visited;
        std::map<Face_handle, K::FT> maxSize;

        Triangulation::Face_circulator f = t.incident_faces(t.infinite_vertex());
        do {
            maxSize[f] = K::FT(std::numeric_limits<double>::max());
            for (int i = 0; i < 3; i++) {
                auto v1 = f->vertex(i % 3);
                auto v2 = f->vertex((i + 1) % 3);
                int neigh = (i + 2) % 3;
                Face_handle face = f->neighbor(neigh);
                if (t.is_infinite(face)) {
                    continue;
                }
                K::FT dist = CGAL::squared_distance(v1->point(), v2->point());
                maxSize[face] = dist;
//                std::cerr << "Adding with " << dist << std::endl;
                visited[face] = true;
                toVisit.push(Entry(face, dist));
            }
        } while (++f != t.incident_faces(t.infinite_vertex()));

//        for (Face_iterator start = t.all_faces_begin(); start != t.all_faces_end(); ++start) {
//            //
//            Face_handle curFace = *start;
//            if (!t.is_infinite(curFace)) {
//                continue;
//            }
//
//            for (int i = 0; i < 3; i++) {
//                auto v1 = curFace->vertex(i % 3);
//                auto v2 = curFace->vertex((i + 1) % 3);
//                int neigh = (i + 2) % 3;
//                Face_handle face = curFace->neighbor(neigh);
//                double dist = CGAL::squared_distance(v1->point(), v2->point());
//                toVisit.push(Entry(face, dist));
//            }
//        }


        while (!toVisit.empty()) {
            Entry cur = toVisit.top();
            Face_handle curFace = cur.fh;
            toVisit.pop();
//
//            std::cerr << "Moving to " << cur.size << ": ";
//                        for (int i = 0; i < 3; i++) {
//                std::cerr << curFace->vertex(i)->point() << ", ";
//            }
//            std::cerr << std::endl;

            maxSize[curFace] = std::max(maxSize[curFace], cur.size);

            for (int i = 0; i < 3; i++) {
                auto v1 = curFace->vertex(i % 3);
                auto v2 = curFace->vertex((i + 1) % 3);
                int neigh = (i + 2) % 3;
                Face_handle face = curFace->neighbor(neigh);
                if (t.is_infinite(face)) {
                    continue;
                }
                K::FT dist = CGAL::squared_distance(v1->point(), v2->point());
                K::FT withPrev = std::min(dist, cur.size);
                maxSize[face] = std::max(maxSize[face], withPrev);
                if (visited[face]) {
                    continue;
                }
                visited[face] = true;
                toVisit.push(Entry(face, maxSize[face]));
            }
        }

        // debug
//        for (auto iter = maxSize.begin(); iter != maxSize.end(); iter++) {
//            for (int i = 0; i < 3; i++) {
//                std::cerr << iter->first->vertex(i)->point() << ", ";
//            }
//            std::cerr << iter->second << std::endl;
//        }

        // Every face should have a maxSize now...
        int m; std::cin >> m;
        for (int i = 0; i < m; i++) {
            Point person;
            std::cin >> person;
             long d; std::cin >> d;
             K::FT dist = K::FT(d);
            // Probably inefficient
            Face_handle start = t.locate(person);

//            std::cerr << i << ": " << maxSize[start] << std::endl;
//            for (int a = 0; a < 3; a++) {
//                std::cerr << start->vertex(a)->point() << ", ";
//            }

            Point nearestInfected = t.nearest_vertex(person)->point();
            K::FT d1 = CGAL::squared_distance(person, nearestInfected);
            if (d1 >= dist && maxSize[start] >= dist * 4.0) {
                // we are good
                std::cout << "y";
            } else {
//                std::cout << (d1 >= d) << " " << maxSize[start] << " >= " << d << std::endl;
                std::cout << "n";
            }
        }
        std::cout << std::endl;

    }
}