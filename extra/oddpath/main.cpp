//
// Created by Hidde Lycklama on 1/24/19.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        boost::no_property,				// interior properties of vertices
        boost::property<boost::edge_weight_t, int> 		// interior properties of edges
>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t


// Functions
// =========
void testcase() {

    int n, m; std::cin >> n >> m;

    int s, t; std::cin >> s >> t;

    // Create Graph, Vertices and Edges
    // ================================
    Graph G(4 * n);	// creates an empty graph on n vertices
    WeightMap weightmap = boost::get(boost::edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38

    for (int i = 0; i < m; i ++ ){
        int u, v, w;
        std::cin >> u >> v >> w;
        Edge e; bool s;

        // 4 levels?
        // 0: even step, even w (starting position)
        // 1: odd step, even w
        // 2: even step, odd w
        // 3: odd step, odd w
        if (w % 2 == 0) {
            // w is even .. only change the step
            boost::tie(e, s) = boost::add_edge((4 * u), (4 * v + 1), G);
            weightmap[e] = w;
            boost::tie(e, s) = boost::add_edge((4 * u + 1), (4 * v), G);
            weightmap[e] = w;
            boost::tie(e, s) = boost::add_edge((4 * u + 2), (4 * v + 3), G);
            weightmap[e] = w;
            boost::tie(e, s) = boost::add_edge((4 * u + 3), (4 * v + 2), G);
            weightmap[e] = w;
        } else {
            // w is uneven .. taking this edge moves to other level
            boost::tie(e, s) = boost::add_edge((4 * u), (4 * v + 3), G);
            weightmap[e] = w;
            boost::tie(e, s) = boost::add_edge((4 * u + 3), (4 * v), G);
            weightmap[e] = w;
            boost::tie(e, s) = boost::add_edge((4 * u + 2),(4 * v + 1), G);
            weightmap[e] = w;
            boost::tie(e, s) = boost::add_edge((4 * u + 1), (4 * v + 2), G);
            weightmap[e] = w;
        }

//        std::cerr << "Adding edge " << (4 * u) << " to " << (4 * v + 1) << std::endl;
    }

    // dijkstraaaa
    int V=num_vertices(G);
    std::vector<Vertex> predmap(V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    boost::dijkstra_shortest_paths(G, s * 4, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter

//    for (int i = 0; i < n; i++) {
//        std::cerr << "Node " << i << ": ";
//        for (int j = 4 * i; j < 4 * i + 4; j++) {
//            std::cerr << distmap[j] << " ";
//        }
//        std::cerr << std::endl;
//    }
//
//    Vertex cur = 4 * t + 3;
//    while (cur != s) {
//        std::cerr << cur << " " << std::endl;
//        cur = predmap[cur];
//    }

     int dist = distmap[(4 * t) + 3]; // we want odd path, odd weight
     if (dist == INT_MAX) {
         std::cout << "no" << std::endl;
     } else {
         std::cout << dist << std::endl;
     }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }

    return 0;
}