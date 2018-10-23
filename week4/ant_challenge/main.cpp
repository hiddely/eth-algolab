//
// Created by Hidde Lycklama on 10/17/18.
//

#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <unordered_map>
#include <limits>

using namespace boost;

typedef std::vector<int> InsectWeight;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        boost::no_property,				// interior properties of vertices
        boost::property<boost::edge_weight_t, int> 		// interior properties of edges
>					Graph;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).


void testcase() {
    int n, e, s, a, b;
    std::cin >> n >> e >> s >> a >> b;

    Graph G(n);
    std::vector<Edge> edges(e);
    std::vector<InsectWeight> insectWeights(e);

    WeightMap weightmap = boost::get(boost::edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38
    for (int i = 0; i < e; i++) {
        int v, u;
        std::cin >> v >> u;
        Edge e; bool success;
        boost::tie(e, success) = boost::add_edge(v, u, G);
        edges[i] = e;
        for (int a = 0; a < s; a++) {
            int iw; std::cin >> iw;
            insectWeights[i].push_back(iw);
        }
    }

    std::vector<int> minWeights(e, std::numeric_limits<int>::max());

    for (int i = 0; i < s; i++) {
        int start; std::cin >> start; // use the hive as start for best runtime

        for (int a = 0; a < e; a++) {
            weightmap[edges[a]] = insectWeights[a][i];
        }

        std::vector<Vertex> primpredmap(n);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
        boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),
                boost::root_vertex(start));	// Prim from *vertices(G).first (usually 0)

        for (int i = 0; i < n; ++i) {
            if (primpredmap[i] != i) {
                Edge e; bool success;
                boost::tie(e, success) = boost::edge(i, primpredmap[i], G);

                ptrdiff_t pos = std::distance(edges.begin(), std::find(edges.begin(), edges.end(), e));
                minWeights[pos] = std::min(minWeights[pos], weightmap[edges[pos]]);
            }
        }
    }

    for (int i = 0; i < e; i++) {
        weightmap[edges[i]] = minWeights[i];
    }

    // We have the MMST with updated weights for all edges ...
    // now just traverse from a to b shortest path

    // DIJKSTRA!

    std::vector<Vertex> predmap(n);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap(n);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    Vertex start = a;
    boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
    std::cout << distmap[b] << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t > 0) {
        testcase();
        t--;
    }
}