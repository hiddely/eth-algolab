//
// Created by Hidde Lycklama on 11/23/18.
//

#include <iostream>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_weight_t, int,
        property<edge_capacity_t, int,
            property<edge_residual_capacity_t, int,
                property<edge_reverse_t, Traits::edge_descriptor> > > > > Graph;

typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t>::type EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type EdgeResidualMap;
typedef property_map<Graph, edge_reverse_t>::type EdgeReverseMap;

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &cMap;
    EdgeWeightMap &wMap;
    EdgeReverseMap &revMap;

public:
    EdgeAdder(Graph &a, EdgeCapacityMap &b, EdgeWeightMap &c, EdgeReverseMap &d): G(a), cMap(b), wMap(c), revMap(d) {}

    void addEdge(int u, int v, int c, int w) {
        Edge e, rev_e;
        bool s;
        tie(e, s) = add_edge(u, v, G);
        tie(rev_e, s) = add_edge(v, u, G);
        cMap[e] = c;
        cMap[rev_e] = 0;
        wMap[e] = w;
        wMap[rev_e] = -w;
        revMap[e] = rev_e;
        revMap[rev_e] = e;
    }
};

void testcase() {
    int n, m, a, s, c, d;
    std::cin >> n >> m >> a >> s >> c >> d;

    Graph G(n + 2);
    EdgeCapacityMap capacityMap = get(edge_capacity, G);
    EdgeWeightMap weightMap = get(edge_weight, G);
    EdgeResidualMap residualMap = get(edge_residual_capacity, G);
    EdgeReverseMap reverseMap = get(edge_reverse, G);
    EdgeAdder adder(G, capacityMap, weightMap, reverseMap);

    Vertex start = n;
    Vertex sink = n + 1;

    for (int i = 0; i < m; i++) {
        char type; std::cin >> type;
        int u, v, w;
        std::cin >> u >> v >> w;
        if (type == 'S') {
            // slope, one way
            adder.addEdge(u, v, INT_MAX, w);
        } else if (type == 'L') {
            // lift, both ways
            adder.addEdge(u, v, INT_MAX, w);
            adder.addEdge(v, u, INT_MAX, w);
        } else {
            std::cout << "Invalid input: " << type;
            abort();
        }
    }

    for (int i = 0; i < a; i++) {
        int agent; std::cin >> agent;
//        std::cout << "edge from " << start << " to " << a
        adder.addEdge(start, agent, 1, 0);
    }
    for (int i = 0; i < s; i++) {
        int shelter; std::cin >> shelter;
        adder.addEdge(shelter, sink, 1, d);
    }


//    for (int i = 0; i <)

////    std::vector<int> distanceMap(n + 2);
//
    successive_shortest_path_nonnegative_weights(G, start, sink
//            distance_map(
////                    make_iterator_property_map(
////                            distanceMap.begin(),
////                            boost::get(boost::vertex_index, G)
////                            )
////                            )
                            );

//    // follow it all
    std::queue<Vertex> toVisit;
    std::vector<int> distances(n + 2);
    std::vector<bool> visited(n + 2);
    toVisit.push(start);
    visited[start] = true;
    while (!toVisit.empty()) {
        Vertex V = toVisit.front();
        toVisit.pop();

        OutEdgeIt e, eend;
        for (tie(e, eend) = out_edges(V, G); e != eend; e++) {
            Vertex edgeTarget = target(*e, G);
//            if (visited[edgeTarget]) {
//                continue;
//            }
            if (capacityMap[*e] - residualMap[*e] > 0) {
                // there is flow !!!
                if (distances[edgeTarget] == 0 || distances[edgeTarget] > distances[V] + weightMap[*e]) {
                    distances[edgeTarget] = distances[V] + weightMap[*e];
                }
                visited[edgeTarget] = true;
                toVisit.push(edgeTarget);
            }
        }
    }

    std::cout << distances[sink] << std::endl;
//    int max = 0;
//    for (int i = 0; i < n + 2 - 1; i++) {
//        std::cout << "From s to " << i << " " << distanceMap[i] << std::endl;
//        max = std::max(max, distanceMap[i]);
//    }

//    std::cout << find_flow_cost(G) << std::endl;
//    std::cout << max << std::endl;

}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}