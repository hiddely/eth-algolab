//
// Created by Hidde Lycklama on 12/24/18.
//

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/graphviz.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
boost::property<boost::vertex_name_t, int>,
        boost::property<boost::edge_capacity_t, long,
                boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::property_map<Graph, boost::vertex_name_t>::type        VertexNameMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;

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

struct VertexState {
    Vertex v;

    VertexState(Vertex a): v(a) {};

    bool operator < (const VertexState &other) const {
        return this->v < other.v;
    }
};

void testcase() {
    int numRequests, numStations;
    std::cin >> numRequests >> numStations;

    std::vector<int> cars(numStations);
    for (int i = 0; i < numStations; i++) {
        std::cin >> cars[i];
    }

    Graph G(2);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    VertexNameMap nameMap = boost::get(boost::vertex_name, G);

    Vertex start = 0;
    Vertex sink = 1;

    const int OFFSET = 2;
    const int MAX_PROFIT = 100;
    const int MAX_FLOW = numStations * 100;
    const int MAXT = 100000;

    std::vector<std::map<int, Vertex>> stationNodes(numStations, std::map<int, Vertex>());
    for (int i = 0; i < numStations; i++) {
        eaG.addEdge(start, OFFSET + i, cars[i], 0);
        stationNodes[i][0] = OFFSET + i;
    }

    for (int i = 0; i < numRequests; i++) {
        int from, to, depart, arrival, profit;
        std::cin >> from >> to >> depart >> arrival >> profit;

        std::map<int, Vertex>* fromNodes = &stationNodes[from - 1];
        std::map<int, Vertex>* toNodes = &stationNodes[to - 1];

        if (fromNodes->find(depart) == fromNodes->end()) {
            // does not exist
            (*fromNodes)[depart] = boost::add_vertex(G);
        }
        if (toNodes->find(arrival) == toNodes->end()) {
            // does not exist
            (*toNodes)[arrival] = boost::add_vertex(G);
        }
        Vertex startNode = (*fromNodes)[depart];
        Vertex finishNode = (*toNodes)[arrival];

//        eaG.addEdge(startNode, sink, MAX_FLOW, MAX_PROFIT); // to the sink
//        eaG.addEdge(finishNode, sink, MAX_FLOW, MAX_PROFIT); // to the sink

//        std::cout << "Adding edge " << startNode << " (" << depart << ")" << " to " << finishNode << " (" << arrival << ")" << " " << profit << std::endl;



        eaG.addEdge(startNode, finishNode, 1, ((arrival - depart) * MAX_PROFIT) - profit); // the path we want it to take
    }

    for (int i = 0; i < numStations; i++) {
        auto vertices = stationNodes[i];
        if (vertices.begin() != vertices.end()) {
            auto iter1 = vertices.cbegin();
//            std::cout << " ayy " << iter1->second << std::endl;
            auto iter2 = ++vertices.cbegin();
            while (iter2 != vertices.cend()) {
//                std::cout << i << " adding edge " << iter1->second << " " << iter2->second << std::endl;

                int cost = (iter2->first - iter1->first) * MAX_PROFIT;

                eaG.addEdge(iter1->second, iter2->second, MAX_FLOW, cost);
                iter1++;
                iter2++;
            }
            eaG.addEdge(iter1->second, sink, MAX_FLOW, (MAXT - iter1->first) * MAX_PROFIT);
        }
    }

//    boost::write_graphviz(std::cout, G);

//    boost::push_relabel_max_flow(G, start, sink);
//    boost::cycle_canceling(G);
    boost::successive_shortest_path_nonnegative_weights(G, start, sink);
    int cost2 = boost::find_flow_cost(G);

    OutEdgeIt e, eend;
    int flow = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(start, G), G); e != eend; ++e) {
//        if (rescapacitymap[*e] - capacitymap[*e] > 0) {
//            std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G)
//                      << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
//
//        }
        flow += rescapacitymap[*e] - capacitymap[*e];
    }

//    std::cout << flow << " " << MAXT * MAX_PROFIT * flow << " " << cost2 << std::endl;

    // every car must travel
    int cost = -MAXT * MAX_PROFIT * flow - cost2;

    std::cout << cost << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T;
    while(T-- > 0) {
        testcase();
    }
    return 0;
}