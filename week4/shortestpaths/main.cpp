//
// Created by Hidde Lycklama on 1/6/19.
//

// Includes
// ========
// STL includes
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

int main() {
    std::ios_base::sync_with_stdio(false);
    int n, m, q;
    std::cin >> n >> m >> q;

    Graph G(n);	// creates an empty graph on n vertices
    WeightMap weightmap = boost::get(boost::edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38

    for (int i = 0; i < n; i++) {
        // read vertices
        int lat, lon;
        std::cin >> lat >> lon;
    }

    for (int i = 0; i < m; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        Edge e; bool s;
        boost::tie(e, s) = boost::add_edge(u, v, G);
        weightmap[e] = w;
    }

    for (int i = 0; i < q; i++) {
        int s, b;
        std::cin >> s >> b;

        // Dijkstra shortest paths
        // =======================
        std::vector<Vertex> predmap(n);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
        std::vector<long> distmap(n);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
        Vertex start = s;
        boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
                                       boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                               distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter

        long dist = distmap[b];
        if (dist != LONG_MAX) {
            std::cout << dist << std::endl;
        } else {
            std::cout << "unreachable" << std::endl;
        }
    }

    return 0;
}