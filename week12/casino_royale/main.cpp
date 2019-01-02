
//
// Created by Hidde Lycklama on 12/28/18.
//

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
                boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap)
            : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};

void testcase() {
    int N, M, L;
    std::cin >> N >> M >> L;

    Graph G(N - 1 + 2);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    const Vertex start = 0;
    const Vertex end = 1;
    const int OFFSET_E = 2;
//    const int OFFSET_M = OFFSET_E + N - 1;

    const int MAX_COST = 128; // 128

    std::vector<int> F(M);
//    std::vector<

    for (int i = 0; i < M; i++) {
        int x, y, p;
        std::cin >> x >> y >> p;

        // find the edges
//        for (int a = x; a < y; a++) {
//            eaG.addEdge(OFFSET_M + i, OFFSET_E + a, 1, MAX_COST - p);
//        }
        eaG.addEdge(OFFSET_E + x, OFFSET_E + y, 1, ((y - x) * MAX_COST) - p);
//        F[i] = y - x;
//        std::cerr << "Adding " << OFFSET_M + i << " to " << y - x << std::endl;
    }

    // edges
    for (int i = 1; i < N; i++) {
//        eaG.addEdge(start, OFFSET_E + i, L, 0);
        eaG.addEdge(OFFSET_E + i - 1, OFFSET_E + i, L, MAX_COST);
    }
    eaG.addEdge(start, OFFSET_E, L, 0);
    eaG.addEdge(OFFSET_E + N - 1, end, L, 0);

    boost::successive_shortest_path_nonnegative_weights(G, start, end);
    int cost2 = boost::find_flow_cost(G);

//    int edited =

//    int flow1 = boost::push_relabel_max_flow(G, start, end);
//    boost::cycle_canceling(G);
//    int cost1 = boost::find_flow_cost(G);

//    int prio = 0;
    int t_flow = 0;
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(end,G), G); e != eend; ++e) {
//        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G)
//                  << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        t_flow += rescapacitymap[*e] - capacitymap[*e];
//        if (boost::target(*e, G) - OFFSET_M >= 0 && rescapacitymap[*e] - capacitymap[*e] == F[boost::target(*e, G) - OFFSET_M]) {
//            prio += weightmap[*e] + MAX_COST;
//        }
    }
    int edited = -cost2 + (MAX_COST * t_flow * (N - 1));

    std::cout << edited << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}