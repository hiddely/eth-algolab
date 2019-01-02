//
// Created by Hidde Lycklama on 12/27/18.
//

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <stack>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property <boost::edge_weight_t, long> > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeWeightMap &weightmap;

public:
    EdgeAdder(Graph &G, EdgeWeightMap &weightmap)
            : G(G), weightmap(weightmap) {}

    void addEdge(int u, int v, long w) {
        Edge e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
//        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        weightmap[e] = w; // new!
//        capacitymap[rev_e] = 0;
//        weightmap[rev_e] = -w; // new
//        revedgemap[e] = rev_e;
//        revedgemap[rev_e] = e;
    }
};

void testcase(int t) {
    int N, M, K;
    long X;
    std::cin >> N >> M >> X >> K;

    Graph G(N);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
//    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
//    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, weightmap);

    const int Max = INT_MAX;
//    std::cout << Max << std::endl;

    for (int i = 0; i < M; i++) {
        int u, v, p;
        std::cin >> u >> v >> p;
        eaG.addEdge(u, v, Max - p);
//        if (t == 18) {
//            std::cerr << i << " Edge " << u << " " << v << std::endl;
//        }
//        if (u > N - 1 || v > N - 1) {
//            std::cerr << "Out of bonuds" << std::endl;
//            abort();
//        }
    }

//    if (t != 18) {
//        return;
//    }

    const Vertex start = 0;

    assert(N == boost::num_vertices(G));
    std::vector<Vertex> predmap(N);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<long> distmap(N);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter

    std::queue<Vertex> toVisit;
    std::vector<bool> visited(N);

    visited[start] = true;
    toVisit.push(start);
    Vertex f;
    bool found = false;
    std::vector<int> depth(N);
    depth[start] = 0;
    while (!toVisit.empty()) {
        Vertex n = toVisit.front();
        toVisit.pop();

//        std::cerr << n << ": " << (depth[n] * Max) - distmap[n] << std::endl;

        if ((((long)depth[n]) * Max) - distmap[n] >= X) {
            // found!
            f = n;
            found = true;
            break;
        }

        OutEdgeIt e, eend;
        for (boost::tie(e, eend) = boost::out_edges(n, G); e != eend; e++) {
            Vertex t = boost::target(*e, G);
//            if (visited[t] && t != start) {
//                // Cycle!
//                std::cerr << "Cycle " << std::endl;
//                abort();
//            }
            if (!visited[t]) {
                //
                depth[t] = depth[n] + 1;
                toVisit.push(t);
                visited[t] = true;
            }
        }
    }

    if (!found) {
        std::cout << "Impossible" << std::endl;

//        for (int i = 0; i < N; i++) {
//            std::cerr << i << ": " << distmap[i] << " " << depth[i] << " " << (((long)depth[i]) * Max) - ((long)distmap[i]) << std::endl;
//        }
    } else {
        // find predecessor
//        Vertex pre = f;
////        std::cerr << pre << std::endl;
//        int steps = 0;
//        while (pre != start) {
//            pre = predmap[pre];
//            steps++;
//        }
        std::cout << depth[f] << std::endl;
    }

//    bool found = false;
//    for (int i = 0; i < N; i++) {
//        if (isEnd[i]) {
//            // k moves
//            if (points[i] >= X) {
//                found = true;
//                break;
//            }
//        }
//    }
//    if (found) {
//        std::cout << K << std::endl;
//    } else {
//        std::cout << "Impossible" << std::endl;
//    }
}

int main() {
    int t;
    std::cin >> t;
    while (t-- > 0) {
        testcase(t);
    }
    return 0;
}