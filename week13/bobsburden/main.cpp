//
// Created by Hidde Lycklama on 1/9/19.
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
#include <boost/graph/graphviz.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        boost::no_property,				// interior properties of vertices
        boost::property<boost::edge_weight_t, double> 		// interior properties of edges
>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

void testcase() {
    int k; std::cin >> k;

    const int numBalls = (k * (k + 1) / 2);
    Graph G(numBalls);	// creates an empty graph on n vertices
    WeightMap weightmap = boost::get(boost::edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38

    std::vector<int> startIndexOnLine(k);
    startIndexOnLine[0] = 0;
    for (int i = 1; i < k; i++) {
        startIndexOnLine[i] = startIndexOnLine[i - 1] + i;
    }

    std::vector<int> ballWeight(numBalls);
    for (int i = 0; i < k; i++) {
        for (int a = 0; a < i + 1; a++) {
            int v; std::cin >> v;
            ballWeight[startIndexOnLine[i] + a] = v;
        }
    }

    for (int i = 0; i < k; i++) {
        for (int a = 0; a < i + 1; a++) {
            int index = startIndexOnLine[i] + a;
            if (i > 0) {
                // not on first line
                Edge e; bool s;
                if (a < i) {
                    // not on the left
                    Vertex rightTop = startIndexOnLine[i - 1] + a;
                    double edgeWeight = ((double) (ballWeight[rightTop] + ballWeight[index])) / 2;
                    boost::tie(e, s) = boost::add_edge(index, rightTop, G);
                    weightmap[e] = edgeWeight;
//                    std::cerr << "Weights: " << ballWeight[rightTop] << " " << ballWeight[index] << std::endl;
                    std::cerr << "Adding edge from " << index << " to " << rightTop;
                    std::cerr << " with weight " << edgeWeight << std::endl;

                    // add edge to the side
                    Vertex rightSide = index + 1;
                    edgeWeight = ((double) (ballWeight[rightSide] + ballWeight[index])) / 2;
                    boost::tie(e, s) = boost::add_edge(index, rightSide, G);
                    weightmap[e] = edgeWeight;
                    std::cerr << "Adding edge from " << index << " to " << rightSide;
                    std::cerr << " with weight " << edgeWeight << std::endl;
                }
                if (a > 0) {
                    // not on the right
                    Vertex leftTop = startIndexOnLine[i - 1] + a - 1;
                    double edgeWeight = ((double) (ballWeight[leftTop] + ballWeight[index])) / 2;
                    boost::tie(e, s) = boost::add_edge(index, leftTop, G);
                    weightmap[e] = edgeWeight;
                    std::cerr << "Adding edge from " << index << " to " << leftTop;
                    std::cerr << " with weight " << edgeWeight << std::endl;
                }
            }
        }
    }

    // Dijkstra shortest paths
    // =======================
    std::vector<Vertex> predmap1(numBalls);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap1(numBalls);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    Vertex start1 = 0;
    boost::dijkstra_shortest_paths(G, start1, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap1.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap1.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter

    std::vector<Vertex> predmap2(numBalls);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap2(numBalls);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    Vertex start2 = startIndexOnLine[k - 1];
    boost::dijkstra_shortest_paths(G, start2, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap2.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap2.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter

    std::vector<Vertex> predmap3(numBalls);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap3(numBalls);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    Vertex start3 = startIndexOnLine[k - 1] + k - 1;
    boost::dijkstra_shortest_paths(G, start3, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap3.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap3.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter

    for (int i = 0; i < numBalls; i++) {
        std::cerr << ballWeight[i] << "'s dist to 0: " << distmap1[i] + distmap2[i] + distmap3[i] << std::endl;
    }
    Vertex end = startIndexOnLine[k - 1];
    while (end != start1) {
        std::cerr << ballWeight[end] << " ";
        end = predmap1[end];
    }
    std::cerr << std::endl;
    end = startIndexOnLine[k - 1] + k - 1;
    while (end != start1) {
        std::cerr << ballWeight[end] << " ";
        end = predmap1[end];
    }
//    boost::write_graphviz(std::cerr, G);
}

int main() {
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}