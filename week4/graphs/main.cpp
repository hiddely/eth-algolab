//
// Created by Hidde Lycklama on 10/11/18.
//

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <vector>
#include <tuple>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
        boost::property<boost::edge_weight_t, int> > Graph;

typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;
typedef Graph::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).


int longestPath(Vertex &start, Graph &G, std::vector<Edge> &spanning_tree, WeightMap &weights) {
    int max = 0;
    for (size_t i = 0; i < spanning_tree.size(); ++i) {
        Edge e = spanning_tree[i];
        Vertex u = boost::source(e, G);
        if (u == start) { // find the edges from this vertex
            // visit target
            Vertex v = boost::target(e, G);
            int l = weights[e] + longestPath(v, G, spanning_tree, weights);
            max = std::max(max, l);
        }
//        std::cout << u << " -- " << v << " (weight " << weights[e] << ")\n";
    }
    return max;
}

void testcase() {
    int n; std::cin >> n;
    int m; std::cin >> m;
    Graph G(n);
    WeightMap weights = boost::get(boost::edge_weight, G);
    Vertex start = 0;
    for (int i = 0; i < m; i++) {
        int v; std::cin >> v;
        int u; std::cin >> u;
        int w; std::cin >> w;
        Edge e;
        bool added;
        boost::tie(e, added) = boost::add_edge(v, u, G);
        weights[e] = w;
    }

//    kruskal_minimum_spanning_tree(G, std::back_inserter(spanning_tree));
//    int sum = 0;
//    for (int i = 0; i < n - 1; i++) {
//        sum += weights[spanning_tree[i]];
//    }

// Compute minimum spanning tree using Prim's algorithm
    std::vector<Vertex> primpredmap(n);
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)));

    int totalweight = 0;
    for(int i = 0; i < n; i++) {
        if (primpredmap[i] != i) {
            Edge e; bool success;
            boost::tie(e, success) = boost::edge(i, primpredmap[i], G);
            totalweight += weights[e];
        }
    }

    std::vector<Vertex> p_map(n);
    std::vector<int> d_map(n);

    boost::dijkstra_shortest_paths(G, start,
            boost::predecessor_map(boost::make_iterator_property_map(p_map.begin(), boost::get(boost::vertex_index, G))).
            distance_map(boost::make_iterator_property_map(d_map.begin(), boost::get(boost::vertex_index, G))));

    int max_dist = 0;
    for(int i = 0; i < n; ++i) {
        if (d_map[i] < INT_MAX) {
            if (d_map[i] > max_dist) {
                max_dist = d_map[i];
            }
        }
    }

    std::cout << totalweight << " " << max_dist << std::endl;
}

int main() {
    int t; std::cin >> t;
    for (; t > 0; t--) {
        testcase();
    }
}