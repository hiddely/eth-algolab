//
// Created by Hidde Lycklama on 10/17/18.
//

//
// Created by Hidde Lycklama on 10/17/18.
//

#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <unordered_map>
#include <limits>
#include <stack>
#include <list>

using namespace boost;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,	boost::no_property,				// interior properties of vertices
        boost::property<boost::edge_weight_t, bool> >					Graph;
//typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
//        boost::property<boost::edge_weight_t, int> > Graph;

typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::property_map<Graph, boost::vertex_index_t>::type VertexIndexMap;

struct my_visitor : boost::default_dfs_visitor {

    WeightMap &weights;
    std::vector<Vertex> &pred_map;
    std::set<Edge> visited;

    explicit my_visitor(WeightMap &w, std::vector<Vertex> &predMap): weights(w), pred_map(predMap) {};

    void tree_edge(const Edge &e, const Graph &g) {
//        std::cout << "Tree edge: " << e << std::endl;
        visited.insert(e);
        Vertex s = source(e, g);
        Vertex t = target(e, g);
        std::cout << "Recording " << s << " as predecessor of " << t << std::endl;
        pred_map[t] = s;
    }

    void back_edge(const Edge &e, const Graph &g) {
        if (visited.find(e) != visited.end()) {
            //
//            std::cout << "Encountered the same " << std::endl;
            return;
        }
        Vertex s = source(e, g);
        Vertex t = target(e, g);
//        std::cout << "Back edge: " << e << std::endl;
        weights[e] = false;
//        std::cout << "Marking " << e << " as nonessential, ";
        Vertex r = s;
        while (r != t) {
            Vertex n = pred_map[r];
            Edge en; bool s;
            boost::tie(en, s) = boost::edge(n, r, g);
            if (s) {
                weights[en] = false;
//                std::cout << "marking " << en << " ";
            }
            r = n;
        }
//        while (!edges.empty()) {
//            Edge p = edges.top();
//            edges.pop();
//            weights[p] = false; // mark as nonessential
//            std::cout << "Marking " << p << " ";
//            Vertex ss = source(p, g);
//            if (t == ss) {
//                break; //
//            }
//        }
//        std::cout << std::endl;
    }
};

void testcase() {
    int n, m;
    std::cin >> n >> m;

    Graph G(n);
    WeightMap weightMap = boost::get(boost::edge_weight, G);
    for (int i = 0; i < m; i++) {
        int v, u;
        std::cin >> v >> u;
        if (u < v) {
            std::swap(v, u);
        }
        Edge e; bool success;
        boost::tie(e, success) = boost::add_edge(v, u, G);
        if (success) {
            weightMap[e] = true; // edge is essential !!!
        }
    }

    std::vector<Vertex> pred_map(n);

    my_visitor vis(weightMap, pred_map);
    depth_first_search(G, visitor(vis));

    std::vector<std::pair<int, int>> list;
    EdgeIt ebeg, eend;
//    std::cout << "IT" << std::endl;
    for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {	// edges(G) returns a pair of iterators which define a range of all edges.
        // For undirected graphs, each edge is visited once, with some orientation.
        // ebeg is EdgeIterator, *ebeg is EdgeDescriptor
//        Vertex u = source(*ebeg, G);
//        Vertex v = target(*ebeg, G);
//        if (predmap[u] == v || predmap[v] == u)
//            std::cout << u << " -- " << v << " (weight " << weightmap[*ebeg] << ")\n";
//
        if (weightMap[*ebeg]) {
            Vertex u = source(*ebeg, G);
            Vertex v = target(*ebeg, G);
            std::pair<int, int> x(u, v);
            list.push_back(x);
        }
    }
//    std::cout << "ET" << std::endl;
    std::cout << list.size() << std::endl;
    std::sort(list.begin(), list.end(), [](const std::pair<int, int> &e1, const std::pair<int, int> &e2) -> bool {
        int ei1 = e1.first;
        int ei2 = e1.second;
        int ej1 = e2.first;
        int ej2 = e2.second;

//        return false;
        if (ei1 == ej1) {
            return ei2 < ej2;
        }
        return ei1 < ej1;
    });
    auto iter = list.cbegin();
    for (; iter != list.cend(); iter++) {
        int u = iter->first;
        int v = iter->second;
        std::cout << u << " " << v << std::endl;
    }
}

int main() {
    int t; std::cin >> t;
    while (t > 0) {
//        std::cout << t << std::endl;
        testcase();
        t--;
    }
}