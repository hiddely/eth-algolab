//
// Created by Hidde Lycklama on 11/19/18.
//

#include <iostream>
#include <vector>
#include <stack>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 Point;

//typedef std::pair<Vertex_handle, Vertex_handle> SE;

struct Node {
    Point vertex;
    struct Node *parent;
    Node(Point a): vertex(a), parent(this) {
    }
    Node() = default;

    bool operator == (const Node &n) const {
        return this->vertex == n.vertex;
    }

    bool operator < (const Node &n) const {
        return this->vertex < n.vertex;
    }
};
struct Edge {
    Point n1;
    Point n2;
    K::FT squared_distance;
    Edge(Point &a, Point &b): n1(a), n2(b) {
        squared_distance = CGAL::squared_distance(n1, n2);
    }
    Edge() = default;

    bool operator < (const Edge &e) const {
        return this->squared_distance < e.squared_distance;
    }
};

Node* find(Node* node) {
    Node *prev;
    Node *c = node;
    while (c->parent != c) {
        prev = c;
        c = c->parent;
        prev->parent = c->parent;
    }
    return c;
}

Node* uni(Node* x, Node* y) { // naive
    Node* rootX = find(x);
    Node* rootY = find(y);

    if (rootX == rootY) {
        return rootX;
    }

//    std::cout << "Unioning " << rootY->vertex << " with " << rootX->vertex << std::endl;
    rootY->parent = rootX;

    return rootX;
}

struct Forest {
    std::vector<Node> nodes;
    std::map<Point, Node*> lookupMap;

    Forest(int n) {
        nodes = std::vector<Node>(n);
    }

    void add_node(Point p, int index) {
        nodes[index] = Node(p);
        nodes[index].parent = &nodes[index];
        lookupMap[p] = &nodes[index];
    }

    Node* lookup(Point p) {
        return lookupMap[p];
    }
};

void testcase() {
    int n, m; long p;
    std::cin >> n >> m >> p;

    std::vector<Point> jammers(n);
    Forest limited(n);
    Forest mst(n);
    Forest c(n);
    for (int i = 0; i < n; i++) {
        std::cin >> jammers[i]; // should work
        limited.add_node(jammers[i], i);
        mst.add_node(jammers[i], i);
        c.add_node(jammers[i], i);
//        std::cout << "Adding node " << jammers[i] << std::endl;
    }

    Triangulation t;
    t.insert(jammers.begin(), jammers.end()); // construct triangulation for NN Graph

    std::vector<Edge> edges;
    for (Edge_iterator iterator = t.finite_edges_begin(); iterator != t.finite_edges_end(); iterator++) {
        Triangulation::Vertex_handle v1 = iterator->first->vertex((iterator->second + 1) % 3);
        Triangulation::Vertex_handle v2 = iterator->first->vertex((iterator->second + 2) % 3);

        auto e = Edge(v1->point(), v2->point());
        edges.push_back(e);
    }
    std::sort(edges.begin(), edges.end());

    // Pick first
    double maxEdge = 0;
    auto limitedIter = edges.cbegin();
    for (; limitedIter != edges.cend(); limitedIter++) {
        if (limitedIter->squared_distance > p) {
//            std::cout << "Skipping edge cause too big " << iter->squared_distance << " " << max << std::endl;
            break; // skip, too far
        }
        Node *l1 = limited.lookup(limitedIter->n1);
        Node *l2 = limited.lookup(limitedIter->n2);
        if (find(l1) != find(l2)) {
            maxEdge = std::max(maxEdge, limitedIter->squared_distance);
            uni(l1, l2);
        }
    }

    auto it_b = edges.cbegin();
    auto it_c = edges.cbegin();

    K::FT minForAll = 0;
    K::FT minRequiredDistance = 0;
    for (int i = 0; i < m; i++) {
        Point pointS, pointT;
        std::cin >> pointS >> pointT;

        Triangulation::Vertex_handle start = t.nearest_vertex(pointS);
        Triangulation::Vertex_handle end = t.nearest_vertex(pointT);

        double entryExit = std::max(CGAL::squared_distance(start->point(), pointS), CGAL::squared_distance(end->point(), pointT)) * 4.0;

        maxEdge = std::max(maxEdge, entryExit);

        if (entryExit <= p &&
            find(limited.lookup(start->point())) == find(limited.lookup(end->point()))) {
//            std::cout << start->point() << ": " << find(lookup[start->point()])->vertex << std::endl;
//            std::cout << end->point() << ": " << find(lookup[end->point()])->vertex << std::endl;
            std::cout << "y";

            if (entryExit > minRequiredDistance) {
                minRequiredDistance = entryExit;
            }

            while (it_c != edges.cend() && find(c.lookup(start->point())) != find(c.lookup(end->point()))) {
                Node *l1 = c.lookup(it_c->n1);
                Node *l2 = c.lookup(it_c->n2);
                if (find(l1) != find(l2)) {
                    uni(l1, l2);
                }
                it_c++;
            }
        } else {
            std::cout << "n";
        }

        if (minForAll < entryExit) {
            minForAll = entryExit;
        }

        while (it_b != edges.cend() && find(mst.lookup(start->point())) != find(mst.lookup(end->point()))) {
            Node *l1 = mst.lookup(it_b->n1);
            Node *l2 = mst.lookup(it_b->n2);
            if (find(l1) != find(l2)) {
                uni(l1, l2);
            }
            it_b++;
        }

    }
    std::cout << std::endl;

//    std::cout << std::fixed << std::setprecision(0) << maxEdge << std::endl;

    if (it_b != edges.cbegin() && (it_b-1)->squared_distance > minForAll) {
        minForAll = (it_b-1)->squared_distance;
    }
    if (it_c != edges.cbegin() && (it_c-1)->squared_distance > minRequiredDistance) {
        minRequiredDistance = (it_c-1)->squared_distance;
    }
    std::cout << std::fixed << std::setprecision(0) << minForAll << std::endl;
    std::cout << std::fixed << std::setprecision(0) << minRequiredDistance << std::endl;
}

int main() {
    int t; std::cin >> t;
//    t = 1;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}