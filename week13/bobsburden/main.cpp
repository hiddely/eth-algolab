#include <iostream>
#include <vector>
#include <cassert>

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



void testcase() {
    int k; std::cin >> k;
    const int V = (k * (k + 1));
    Graph G(V);
    WeightMap weightmap = boost::get(boost::edge_weight, G);

    std::vector<int> weights(V);

    int counter = 0;
    for (int l = 0; l < k; l++) {
        const int upThisLine = l * (l + 1);
        for (int i = 0; i < l + 1; i++) {
            int w; std::cin >> w;
            Edge e; bool s;
            //const int index =
            const int inIndex = upThisLine + (i * 2);
            const int outIndex = upThisLine + (i * 2)+1;
            boost::tie(e, s) = boost::add_edge(inIndex, outIndex, G);
            assert(s);
            weightmap[e] = w;
            weights[inIndex] = w;

            // std::cerr << i <<  " Adding edge " << inIndex << " to " << outIndex << " w " << w << std::endl;

            if (i < l) {
                // right
                boost::tie(e, s) = boost::add_edge(outIndex, outIndex + 1, G); // Outgoing edge from right
                assert(s);
                weightmap[e] = 0;
                boost::tie(e, s) = boost::add_edge(outIndex + 2, inIndex, G); // Incoming edge from right
            }

            // add edges down and up
            if (l > 0) {
                const int prevLine = upThisLine - ((l - i) * 2);
                if (i < l) {
                    // right top
                    // std::cerr << "Adding edge right top " << outIndex << " " << prevLine << std::endl; //
                    boost::tie(e, s) = boost::add_edge(outIndex, prevLine, G); // outgoing to incoming
                    assert(s);
                    weightmap[e] = 0;
                    boost::tie(e, s) = boost::add_edge(prevLine + 1, inIndex, G); // incoming to outgoing
                }
                if (i > 0) {
                    // left top
                    // std::cerr << i << " Adding edge left top " << outIndex << " " << prevLine - 2<< std::endl;
                    boost::tie(e, s) = boost::add_edge(outIndex, prevLine - 2, G);
                    assert(s);
                    weightmap[e] = 0;
                    boost::tie(e, s) = boost::add_edge(prevLine - 1, inIndex, G); // incoming to outgoing
                }
            }
        }
    }

    std::vector<Vertex> predmap1(V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap1(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    Vertex start1 = 1;
    boost::dijkstra_shortest_paths(G, start1, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap1.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap1.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter

    std::vector<Vertex> predmap2(V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap2(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    Vertex start2 = V - (k * 2) + 1;
    boost::dijkstra_shortest_paths(G, start2, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap2.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap2.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
    std::vector<Vertex> predmap3(V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap3(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    Vertex start3 = V - 1;
    boost::dijkstra_shortest_paths(G, start3, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap3.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap3.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter


    int minSize = INT_MAX;
    int minBall = -1;
    for (int i = 2; i < V - 2; i += 2) {
        if (i == start2) {
            continue;
        }
        // compare balls
        // Vertex cur = i;		//
        // while (cur != start1) {
        //   std::cerr << cur << " ";
        //  cur = predmap1[cur];
        //   }
        // std::cerr << std::endl;
        const int ballWeight = distmap1[i] + distmap2[i] + distmap3[i] + weights[i];
        // std::cerr << i << " " << distmap1[i] << " " << distmap2[i] << " " << distmap3[i] << " " << weights[i] << " = " << ballWeight << std::endl;
        if (ballWeight < minSize) {
            minSize = ballWeight;
            minBall = i;
        }
    }

    std::cout << minSize << std::endl;


}

int main() {
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}