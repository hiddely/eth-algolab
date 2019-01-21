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
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>

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
        weightmap[e] = w; // new!
    }
};

void testcase(int t) {
    int N, M, K;
    long X;
    std::cin >> N >> M >> X >> K;

    Graph G(N);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    EdgeAdder eaG(G, weightmap);

//    std::vector<bool> toke(N, true);

    for (int i = 0; i < M; i++) {
        int u, v, p;
        std::cin >> u >> v >> p;

//        toke[u] = false;

        eaG.addEdge(u, v, p);
    }

    std::vector<std::vector<long> > map(K + 1, std::vector<long> (N, -1) );
    Vertex start = 0;
    map[0][start] = 0;
    for (int i = 0; i < K; i++) {
        for (int r = 0; r < N; r++) {
            if (map[i][r] != -1) {
                // find out edges
                OutEdgeIt e, eend;
                boost::tie(e, eend) = boost::out_edges(r, G);
                if (e == eend) {
                    // we have a toke
                    boost::tie(e, eend) = boost::out_edges(start, G);
                }
                for (; e != eend; e++) {
                    Vertex t = boost::target(*e, G);
                    map[i + 1][t] = std::max(map[i + 1][t], map[i][r] + weightmap[*e]);
//                    std::cerr << i << ", " << r << ": Visiting edge " << r << " -> " << t << ", " << map[i + 1][t] << std::endl;

                    if (map[i + 1][t] >= X) {
                        std::cout << i + 1 << std::endl;
                        return;
                    }
                }
            }
        }
    }
    std::cout << "Impossible" << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t-- > 0) {
        testcase(t);
    }
    return 0;
}