//
// Created by Hidde Lycklama on 1/8/19.
//

#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <limits>
#include <queue>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
//tyepdef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Face_handle Face_handle;
typedef K::Point_2 Point;

std::map<Face_handle, K::FT> distMap;

struct Entry {
    Face_handle f;
    K::FT size;

    Entry(Face_handle a, K::FT s): f(a), size(s) {}

    bool operator < (const Entry &o) const {
//        return distMap[this->f] < distMap[o.f];
        return this->size < o.size;
    }
};


void testcase() {
    int numTrees, numBalloons;
    long treeRadiusH;
    std::cin >> numTrees >> numBalloons >> treeRadiusH;
    K::FT treeRadius = K::FT(treeRadiusH);

    std::vector<Point> pts(numTrees);
    for (int i = 0; i < numTrees; i++) {
        long x, y;
        std::cin >> x >> y;
        pts[i] = Point(x, y);
    }

    Triangulation t;
    t.insert(pts.begin(), pts.end());

    // calculate
    std::priority_queue<Entry, std::vector<Entry>> queue;
    std::map<Face_handle, bool> visited;
    distMap = std::map<Face_handle, K::FT>();

    // infinite vertices
    Triangulation::Face_circulator f = t.incident_faces(t.infinite_vertex());
    do {
        distMap[f] = K::FT(std::numeric_limits<double>::max());
        visited[f] = true;
        for (int i = 0; i < 3; i++) {
            Face_handle n = f->neighbor(i);
            if (t.is_infinite(n)) {
                continue;
            }
            K::FT dist = t.segment(f, i).squared_length();
//            std::cerr << "Pushing " << n->vertex(0)->point() << " " << n->vertex(1)->point() << " " << n->vertex(2)->point() << ": " << dist << std::endl;
//            std::cerr << "From " << f->vertex(0)->point() << " " << f->vertex(1)->point() << " " << f->vertex(2)->point() << std::endl;
//            visited[n] = true;
//            K::FT launchSize = CGAL::squared_distance(t.circumcenter(n), n->vertex(0)->point());
            distMap[n] = dist;
//            queue.push(Entry(n, std::max(launchSize, dist), f));
        }
    } while (++f != t.incident_faces(t.infinite_vertex()));

    for (auto iter = t.all_faces_begin(); iter != t.all_faces_end(); iter++) {
        if (!t.is_infinite(iter)) {
            K::FT launchSize = CGAL::squared_distance(t.circumcenter(iter), iter->vertex(0)->point());
            distMap[iter] = std::max(distMap[iter], launchSize);
            queue.push(Entry(iter, distMap[iter]));
        }
//        } else {
//            distMap[iter] = K::FT(std::numeric_limits<double>::max());
//        }
//        queue.push(Entry(iter, 0, iter));
    }

    while (!queue.empty()) {
        Entry e = queue.top();
        queue.pop();

        if (visited[e.f]) {
            continue;
        }

        visited[e.f] = true;

//        visited[e.f] = true;

//        K::FT launchSize = CGAL::squared_distance(t.circumcenter(e.f), e.f->vertex(0)->point());
//        distMap[e.f] = std::max(distMap[e.f], std::min(e.size, distMap[e.prev]));
//        std::cerr << "Visiting " << t.triangle(e.f) << ": " << distMap[e.f] << std::endl;
////        std::cerr << "Cir " << t.circumcenter(e.f) << " " << launchSize << std::endl;
//        if (!t.is_infinite(e.prev)) {
//            std::cerr << "From " << t.triangle(e.prev) << std::endl;
//        }
        for (int i = 0; i < 3; i++) {
            Face_handle n = e.f->neighbor(i);
            if (t.is_infinite(n)) {
                continue;
            }
            K::FT dist = t.segment(e.f, i).squared_length();
//            K::FT launchSize = CGAL::squared_distance(t.circumcenter(n), n->vertex(0)->point());
            distMap[n] = std::max(distMap[n], std::min(dist, distMap[e.f]));
            if (visited[n]) {
                continue;
            }
            queue.push(Entry(n, distMap[n]));
        }
    }
//    t.weig


//    for (auto iter = distMap.begin(); iter != distMap.end(); iter++) {
//        if (!t.is_infinite(iter->first))
//            std::cerr << iter->first->vertex(0)->point() << " " << iter->first->vertex(1)->point() << " " << iter->first->vertex(2)->point() << ": " << iter->second << std::endl;
//    }

    for (int i = 0; i < numBalloons; i++) {
        long x, y, s;
        std::cin >> x >> y >> s;
        Point p(x, y);

        // Check if inflatable... !!!
        Vertex_handle v = t.nearest_vertex(p);
        Face_handle face = t.locate(p);
        // inflatable
        K::FT dist = CGAL::squared_distance(v->point(), p);
        K::FT bal = K::FT(s) + treeRadius;
        K::FT treeBalloonRadiusSquared = bal * bal;

//        if (i == 16) {
//            std::cerr << p << " and " << v->point() << ": " << dist << " >= " << treeBalloonRadiusSquared << std::endl;
//        }

        K::FT launchDist = 2 * K::FT(s) + 2 * treeRadius;
        K::FT launchDistSquared = launchDist * launchDist;
//        if (i == 16) {
//            std::cerr << distMap[face] << " >= " << (launchDist * launchDist) << std::endl;
//        }

        if (dist >= treeBalloonRadiusSquared && (dist >= launchDistSquared || distMap[face] >= launchDistSquared)) {
            // We may continue, it is inflatable
            // check if it is possible to move to suitable launch zone
//            if (dist >= launchDist * launchDist)
            std::cout << "y";
        } else {
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