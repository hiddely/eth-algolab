//
// Created by Hidde Lycklama on 11/23/18.
//

#include <iostream>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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

    void dijkstraAdd(int u, int v, int w) {
        Edge e;
        bool s;
        tie(e, s) = add_edge(u, v, G);
        wMap[e] = w;
    }
};

bool too_small(int maxCost, std::vector<std::vector<int> > &distanceMatrix, std::vector<int> &agents, std::vector<int> &shelters, int n, int a, int s, int d, int c) {
    Graph H(a + (c * s) + 2);
    EdgeCapacityMap capacityMapH = get(edge_capacity, H);
    EdgeWeightMap weightMapH = get(edge_weight, H);
    EdgeResidualMap residualMapH = get(edge_residual_capacity, H);
    EdgeReverseMap reverseMapH = get(edge_reverse, H);
    EdgeAdder adderH(H, capacityMapH, weightMapH, reverseMapH);

//    std::cout << "Trying " << maxCost << " " << std::endl;

    Vertex start = a + (c * s);
    Vertex sink = a + (c * s) + 1;

    for (int y = 0; y < s; y++) {
        Vertex shelter = shelters[y];
//        std::cout << "E Adding " << shelter << " -> " << sink << std::endl;
        adderH.addEdge(a + y, sink, 1, 0);
        if (c == 2) {
            adderH.addEdge(a + y + s, sink, 1, 0);
        }
    }

    for (int x = 0; x < a; x++) {
        Vertex agent = agents[x];
        adderH.addEdge(start, x, 1, 0);
//        std::cout << "A Adding " << start << " -> " << agent << std::endl;
        for (int y = 0; y < s; y++) {
            Vertex shelter = shelters[y];
            if (distanceMatrix[x][y] + d <= maxCost && distanceMatrix[x][y] < INT_MAX) {
//                std::cout << "Adding edge " << agent << " " << shelter << " " << distanceMatrix[x][y] <<  std::endl;
                adderH.addEdge(x, a + y, 1, 0);
            }
            if (distanceMatrix[x][y] + (2 * d) <= maxCost && c == 2 && distanceMatrix[x][y] < INT_MAX) {
                adderH.addEdge(x, a + s + y, 1, 0);
            }
        }
    }

    int flow = boost::push_relabel_max_flow(H, start, sink);
    return flow != a;
}

void testcase() {
    int n, m, a, s, c, d;
    std::cin >> n >> m >> a >> s >> c >> d;

    Graph G(n);
    EdgeCapacityMap capacityMap = get(edge_capacity, G);
    EdgeWeightMap weightMap = get(edge_weight, G);
    EdgeResidualMap residualMap = get(edge_residual_capacity, G);
    EdgeReverseMap reverseMap = get(edge_reverse, G);
    EdgeAdder adder(G, capacityMap, weightMap, reverseMap);

    for (int i = 0; i < m; i++) {
        char type; std::cin >> type;
        int u, v, w;
        std::cin >> u >> v >> w;
        if (type == 'S') {
            // slope, one way
            adder.dijkstraAdd(u, v, w);

        } else if (type == 'L') {
            // lift, both ways
            adder.dijkstraAdd(u, v, w);
            adder.dijkstraAdd(v, u, w);
        } else {
            std::cout << "Invalid input: " << type;
            abort();
        }
    }

    std::vector<int> agents(a);
    std::vector<int> shelters(s);

    for (int i = 0; i < a; i++) {
        int agent; std::cin >> agent;
//        std::cout << "edge from " << start << " to " << a
        agents[i] = agent;
//        adder.addEdge(start, agent, 1, 0);
    }

    for (int i = 0; i < s; i++) {
        int shelter; std::cin >> shelter;
        shelters[i] = shelter;
    }

    std::vector<std::vector<int> > distanceMatrix(a, std::vector<int>(s));
    // we DIJKSTRA
    for (int i = 0; i < a; i++) {
        std::vector<Vertex> p_map(n);
        std::vector<int> d_map(n);

        Vertex agent = agents[i];

        boost::dijkstra_shortest_paths(G, agent,
                                       boost::predecessor_map(boost::make_iterator_property_map(p_map.begin(), boost::get(boost::vertex_index, G))).
                                               distance_map(boost::make_iterator_property_map(d_map.begin(), boost::get(boost::vertex_index, G))));
        // Check the weightmap

//        distanceMatrix[i] = d_map;

//        adderH.addEdge(start, agent, 1, 0);

        for (int x = 0; x < s; x++) {
            Vertex shelter = shelters[x];
            distanceMatrix[i][x] = d_map[shelter];
        }
//            if (d_map[shelter] != INT_MAX) {
//                std::cout << "Agent " << agent << " -> " << shelter << ": " << d_map[shelter] << std::endl;
//                adderH.addEdge(agent, shelter, 1, d_map[shelter]);
//            }
//        }
    }

    int lmin = 0, lmax = 1;
    while (too_small(lmax, distanceMatrix, agents, shelters, n, a, s, d, c)) {
        lmax *= 2;
    }
    while (lmin != lmax) {
        int p = (lmin + lmax) / 2;
        if (too_small(p, distanceMatrix, agents, shelters, n, a, s, d, c)) {
            lmin = p + 1;
        } else {
            lmax = p;
        }
    }
    std::cout << lmin << std::endl;



//    for (int i = 0; i <)

////    std::vector<int> distanceMap(n + 2);
//
//            distance_map(
////                    make_iterator_property_map(
////                            distanceMap.begin(),
////                            boost::get(boost::vertex_index, G)
////                            )
////                            )
//                            );

////    // follow it all
//    std::queue<Vertex> toVisit;
//    std::vector<int> distances(n + 2);
//    std::vector<bool> visited(n + 2);
//    toVisit.push(start);
//    visited[start] = true;
//    while (!toVisit.empty()) {
//        Vertex V = toVisit.front();
//        toVisit.pop();
//
//        OutEdgeIt e, eend;
//        for (tie(e, eend) = out_edges(V, G); e != eend; e++) {
//            Vertex edgeTarget = target(*e, G);
////            if (visited[edgeTarget]) {
////                continue;
////            }
//            if (capacityMap[*e] - residualMap[*e] > 0) {
//                // there is flow !!!
//                if (distances[edgeTarget] == 0 || distances[edgeTarget] > distances[V] + weightMap[*e]) {
//                    distances[edgeTarget] = distances[V] + weightMap[*e];
//                }
//                visited[edgeTarget] = true;
//                toVisit.push(edgeTarget);
//            }
//        }
//    }
//
//    std::cout << distances[sink] << std::endl;
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