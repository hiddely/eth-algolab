//
// Created by Hidde Lycklama on 11/16/18.
//

#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <queue>

using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_reverse_t, Traits::edge_descriptor,
            property<edge_residual_capacity_t, long,
                property<edge_weight_t, long> > > > > Graph;

typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type EdgeReverseMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_weight_t>::type EdgeWeightMap;
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
    int n, m, s;
    std::cin >> n >> m >> s;

    const int SPACE_BUYERS = 0;
    const int SPACE_ESTATES = n;
    const int SPACE_STATES = n + m;
    const int SPACE_SS = n + m + s;

    Graph G(n + m + s + 2);
    EdgeCapacityMap capacityMap = get(edge_capacity, G);
    EdgeReverseMap  reverseMap = get(edge_reverse, G);
    ResidualCapacityMap residualMap = get(edge_residual_capacity, G);
    EdgeWeightMap weightMap = get(edge_weight, G);
    EdgeAdder adder(G, capacityMap, reverseMap, weightMap);

    Vertex source = SPACE_SS;
    Vertex target = SPACE_SS + 1;

    const int offset = 101;

    for (int i = 0; i < n; i++) {
        adder.addEdge(source, i, 1, 0);
    }

    // 0-2: buyers, 3-5: houses, 6: state, 7: source, 8: sink

    for (int i = 0; i < s; i++) {
        int limitsPerState; // every land can be sold max this
        std::cin >> limitsPerState; // ignore
//        std::cout << "Adding edge for state " << i << " to " << target << " limit " << limitsPerState << std::endl;
        adder.addEdge(SPACE_STATES + i, target, limitsPerState, 0);
    }
    for (int i = 0; i < m; i++) {
        int state; std::cin >> state;
//        std::cout << "Land -> State: " << SPACE_ESTATES + i << " " << SPACE_STATES + state << std::endl;
        adder.addEdge(SPACE_ESTATES + i, SPACE_STATES + state - 1, 1, 0);
    }
    for (int a = 0; a < n; a++) {
        // buyers
        for (int b = 0; b < m; b++) {
            int p; std::cin >> p;
//            std::cout << "Adding edge from " << a << " to " << n + b << std::endl;
            adder.addEdge(a, SPACE_ESTATES + b, 1, offset - p);
        }
    }

    int flow = push_relabel_max_flow(G, source, target);
    std::cout << flow << " ";

//    cycle_canceling(G);
//    int cost1 = boost::find_flow_cost(G);
//
//    std::cout << cost1 << std::endl;

    successive_shortest_path_nonnegative_weights(G, source, target);
    int cost2 = find_flow_cost(G);

    int corrected = flow * offset - cost2;

    std::cout << corrected << std::endl;

//    std::queue<Vertex> Q;
//    std::vector<bool> visited(n + m + 2);
//    Q.push(source);
//    visited[source] = true;
//    while (!Q.empty()) {
//        Vertex from = Q.front();
//        Q.pop();
//
//        std::cout << "Starting at " << from;
//
//        OutEdgeIt e, eend;
//        for (tie(e, eend) = out_edges(from, G); e != eend; e++) {
//            Vertex to = boost::target(*e, G);
//            std::cout << " visting " << to;
//            if (!visited[to] && residualMap[*e] != 0) {
//                std::cout << " (yes) ";
//                visited[to] = true;
//                Q.push(to);
//            }
//        }
//        std::cout <<std::endl;
//    }
//
//    for (int i = 0; i < n + m; i++) {
//        if (visited[i]) {
//            std::cout << i << " ";
//        }
//    }
//    std::cout << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}