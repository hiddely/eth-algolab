//
// Created by Hidde Lycklama on 12/3/18.
//

#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
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
        boost::property<boost::vertex_index2_t, int>,				// interior properties of vertices
        boost::property<boost::edge_weight_t, int> 		// interior properties of edges
>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t
typedef boost::property_map<Graph, boost::vertex_index2_t>::type TempMap;

typedef std::pair<int, int> MinMax;

MinMax find(std::deque<Vertex> &window, TempMap &temp) {
    int min = INT_MAX, max = INT_MIN;
    for (auto iter = window.begin(); iter != window.end(); iter++) {
        min = std::min(min, temp[*iter]);
        max = std::max(max, temp[*iter]);
    }
    return std::pair<int, int>(min, max);
}

void testcase() {
    int n, m, k;
    std::cin >> n >> m >> k;

    Graph G(n);
//    WeightMap weightmap = boost::get(boost::edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38
//    TempMap tempMap = boost::get(boost::vertex_index2, G);

    std::vector<int> temps(n);
    for (int i = 0; i < n; i ++) {
        int temp; std::cin >> temp;
        temps[i] = temp;
    }

    std::vector<int> predmap(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        Edge e; bool s;
        boost::tie(e, s) = boost::add_edge(u, v, G);
        predmap[v] = u;
    }

    // assuming line
    Vertex start = 0;
    Vertex lastStart = 0;
    int min = INT_MAX;
    int max = INT_MIN;
    int currentCount = 0;
    int numRoutes = 0;
    std::set<int> outputs;
    std::vector<Vertex> currents;
    while (start != -1) {

        min = std::min(min, temps[start]);
        max = std::max(max, temps[start]);

        if (max - min > k) {
            // too much risk
//            numRoutes += std::max(0, m - currentCount + 1);
            Vertex p = start;
            for (int i = 0; i < currentCount - m + 1; i++){
                outputs.insert(currents[i]);
            }
            min = temps[start];
            max = temps[start];
            currentCount = 0;
        }

        currents.push_back(start);
        currentCount++;

        OutEdgeIt e, eend;
        Vertex x = -1;
        for (boost::tie(e, eend) = boost::out_edges(start, G); e != eend; e++) {
            Vertex t = boost::target(*e, G);
//            std::cerr << "Setting start " << t << std::endl;
            x = t;
        }
        lastStart = start;
        start = x;
    }

    if (max - min <= k) {
        // too much risk
//            numRoutes += std::max(0, m - currentCount + 1);
        Vertex p = lastStart;
        for (int i = 0; i < m - 1; i++){
            p = predmap[p];
        }
//        std::cerr << start << " Ad2ding " << p << " " << currentCount - m + 1 << std::endl;
        for (int i = 0; i < currentCount - m + 1; i++) {
            outputs.insert(p);
            p = predmap[p];
        }
        min = temps[start];
        max = temps[start];
        currentCount = 0;
    }

//    std::cout << numRoutes << std::endl;
    if (outputs.empty()) {
        std::cout << "Abort mission" << std::endl;
    } else {
        for (auto iter = outputs.cbegin(); iter != outputs.cend(); iter++) {
            std::cout << *iter << " ";
        }
        std::cout <<std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) {
        testcase();
    }
    return 0;
}