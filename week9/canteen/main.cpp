//
// Created by Hidde Lycklama on 11/16/18.
//

#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_weight_t, long,
        property<edge_capacity_t, long,
            property<edge_residual_capacity_t, long,
                property<edge_reverse_t, Traits::edge_descriptor> > > > > Graph;

typedef property_map<Graph, edge_weight_t>::type EdgeWeightMap;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type EdgeResidualMap;
typedef property_map<Graph, edge_reverse_t>::type EdgeReverseMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &cMap;
    EdgeReverseMap &rMap;
    EdgeWeightMap &wMap;

public:
    EdgeAdder(Graph &a, EdgeCapacityMap &b, EdgeReverseMap &c, EdgeWeightMap &d): G(a), cMap(b), rMap(c), wMap(d) {}

    void addEdge(int u, int v, int c, int w) {
        Edge e, rev_e;
        bool s;
        tie(e, s) = add_edge(u, v, G);
        tie(rev_e, s) = add_edge(v, u, G);
        cMap[e] = c;
        cMap[rev_e] = 0;
        rMap[e] = rev_e;
        rMap[rev_e] = e;
        wMap[e] = w;
        wMap[rev_e] = -w;
    }
};

void testcase() {
    int n; std::cin >> n;

    Graph G( n + 2);
    EdgeCapacityMap capacityMap = get(edge_capacity, G);
    EdgeReverseMap  reverseMap = get(edge_reverse, G);
    EdgeResidualMap residualMap = get(edge_residual_capacity, G);
    EdgeWeightMap weightMap = get(edge_weight, G);
    EdgeAdder adder(G, capacityMap, reverseMap, weightMap);

    const int O_PROD = 0;
//    const int O_FREEZER = 2 * n;
    const int O_SS = n;
    Vertex start = O_SS;
    Vertex end = O_SS + 1;

    int totalStudentsToFeed = 0; // max flow we would want

    const int maxPrice = 20;

    for (int i = 0; i < n; i++) {
        int numMenus, cost;
        std::cin >> numMenus >> cost;
        adder.addEdge(start, i, numMenus, cost);
    }
    for (int i = 0; i < n; i++) {
        int numStudents, price;
        std::cin >> numStudents >> price;
        adder.addEdge(i, end, numStudents, maxPrice - price);
        totalStudentsToFeed += numStudents;
    }
    for (int i = 0; i < n - 1; i ++) {
        int freezeCapacity, cost;
        std::cin >> freezeCapacity >> cost;

        // Add edge from prod to const next day
        adder.addEdge(i, i + 1, freezeCapacity, cost);

    }

//    int flow = push_relabel_max_flow(G, start, end);
//    if (flow == totalStudentsToFeed) {
//        std::cout << "possible ";
//    } else {
//        std::cout << "impossible ";
//    }
//    std::cout << flow << " ";

    successive_shortest_path_nonnegative_weights(G, start, end);

    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(start,G), G); e != eend; ++e) {
        s_flow += capacityMap[*e] - residualMap[*e];
    }
    if (s_flow == totalStudentsToFeed) {
        std::cout << "possible ";
    } else {
        std::cout << "impossible ";
    }
    std::cout << s_flow << " ";

    int cost2 = find_flow_cost(G);
    int correction = (s_flow * maxPrice) - cost2;
    std::cout << correction << std::endl;

//    cycle_canceling(G);
//    int cost = find_flow_cost(G);
//    std::cout << -cost << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}