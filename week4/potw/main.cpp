//
// Created by Hidde Lycklama on 10/15/18.
//

#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <set>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Seg;
typedef K::Triangle_2 Tri;
typedef K::Line_2 Line;

Point getPoint() {
    int x, y;
    std::cin >> x >> y;
    return Point(x, y);
}

Line getEdge() {
    return Line(getPoint(), getPoint());
}

std::vector<Line> getEdges() {
    std::vector<Line> edges(3);
    for (int i = 0; i < 3; i++) {
        edges[i] = getEdge();
    }
    return edges;
}

Point getIntersection(Line seg1, Line seg2) {
    if (!CGAL::do_intersect(seg1, seg2)) {
        std::cout << "Error parsing triangle segments" << std::endl;
        throw std::invalid_argument( "received segment" );
    }
    // check intersection
    auto o = CGAL::intersection(seg1, seg2);
    if (const Point* op = boost::get<Point>(&*o)) {
        return *op;
    }
    else if (const Line* os = boost::get<Line>(&*o)) {
        std::cout << "Error parsing triangle segments, found segment" << std::endl;
        throw std::invalid_argument( "received segment" );
    }
}

std::vector<Point> loadPoints() {
    std::vector<Point> points(3);
    std::vector<Line> edges = getEdges();
    points[0] = getIntersection(edges[0], edges[1]);
    points[1] = getIntersection(edges[1], edges[2]);
    points[2] = getIntersection(edges[2], edges[0]);
    return points;
}

void testcase() {
    int m, n;
    std::cin >> m >> n;

    std::vector<Seg> path(m - 1);
    int startX, startY;
    std::cin >> startX >> startY;
    Point start(startX, startY);
    for (int i = 0; i < m - 1; i++) {
        int x, y;
        std::cin >> x >> y;
        Point p(x, y);
        path[i] = Seg(start, p);
        start = p;
    }

    std::vector<Tri> tri(n);
    for (int i = 0; i < n; i++) {
        std::vector<Point> p = loadPoints();
        tri[i] = Tri(p[0], p[1], p[2]);
//        std::cout << "P " << p[0] << " " << p[1] << " " << p[2] << std::endl;
    }

    std::vector<std::set<int>> pathsInTriangles(n, std::set<int>());
    int lowerBound = 0;
    for (int a = 0; a < n; a++) { // triangles
        for (int b = 0; b < m - 1; b++) { // legs
            auto s = tri[a].bounded_side(path[b].source());
            auto t = tri[a].bounded_side(path[b].target());
            if ((s == CGAL::ON_BOUNDED_SIDE || s == CGAL::ON_BOUNDARY) &&
                    (t == CGAL::ON_BOUNDED_SIDE || t == CGAL::ON_BOUNDARY)) {
                pathsInTriangles[a].insert(b);
//                std::cout << path[b] << " >> " << tri[a] << " " << "Found overlap at " << a << std::endl;
            }
        }
    }

    std::vector<int> pathCount(m - 1, 0);

    int a = 0; int b = 0;
    int k = n - 1;
    while (a < n) {
//        std::cout << "Visiting (" << a << "," << b << ")" << std::endl;
        if (b == n || *std::min_element(pathCount.begin(), pathCount.end()) != 0) {
            // incr a
//            std::cout << "lb" << std::endl;
            auto it = pathsInTriangles[a].cbegin();
            while (it != pathsInTriangles[a].cend()) {
                pathCount[*it]--;

                if (pathCount[*it] < 0) {
                    std::cout << "This can never happen" << std::endl;
                }

                it++;
            }
            a++;
        } else {
            // add items to array
//            std::cout << "ub" << std::endl;
            auto it = pathsInTriangles[b].cbegin();
            while (it != pathsInTriangles[b].cend()) {
                pathCount[*it]++;
                it++;
            }
            b++;
        }
        if (*std::min_element(pathCount.begin(), pathCount.end()) != 0) {
            // wew
//            std::cout << "Found candidate!" << a << " " << b << std::endl;
            k = std::min(k, b - a);
        }
    }

    // Tri 1: 0, 1
    // Tri 2: -
    // Tri 3: 1, 2
    // Tri 4: 0

    std::cout << k << std::endl;

    // 0, 10
    // 5, 8

}

int main() {
    int t; std::cin >> t;
    for (; t > 0; t--) {
        testcase();
    }
}