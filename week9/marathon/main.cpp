//
// Created by Hidde Lycklama on 11/16/18.
//

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#include <map>
#include <vector>

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

typedef graph_traits<Graph>::edge_descriptor  Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef graph_traits<Graph>::edge_iterator EdgeIt;

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &cMap;
    EdgeReverseMap &rMap;
    EdgeWeightMap &wMap;

public:
    bool addBoth = false;

    EdgeAdder(Graph &a, EdgeCapacityMap &b, EdgeReverseMap &c, EdgeWeightMap &d): G(a), cMap(b), rMap(c), wMap(d) {}

    void addEdge(int u, int v, int c, int w) {
        Edge e, rev_e;
        bool s;
        tie(e, s) = add_edge(u, v, G);
        tie(rev_e, s) = add_edge(v, u, G);
        cMap[e] = c;
        cMap[rev_e] = addBoth ? c : 0;
        rMap[e] = rev_e;
        rMap[rev_e] = e;
        wMap[e] = w;
        wMap[rev_e] = w;
    }
};

//struct DijkstraVisitor: public default_dijkstra_visitor {
//
//    std::vector<int> &minDistance;
//    std::vector<int> &minFlow;
//    EdgeCapacityMap &capacityMap;
//
//    DijkstraVisitor(EdgeCapacityMap &a, std::vector<int> &b, std::vector<int> &c): capacityMap(a), minDistance(b), minFlow(c) {}
//
//    void edge_relaxed(const Edge &e, const Graph &g) {
//        Vertex v = target(e, g);
//        minFlow[v] = capacityMap[e];
//    }
//
//    void edge_
//
//};

void testcase() {
    int n, m, s, f;
    std::cin >> n >> m >> s >> f;

    Graph G(n);
    EdgeCapacityMap capacityMap = get(edge_capacity, G);
    EdgeReverseMap  reverseMap = get(edge_reverse, G);
    EdgeResidualMap residualMap = get(edge_residual_capacity, G);
    EdgeWeightMap weightMap = get(edge_weight, G);
    EdgeAdder adder(G, capacityMap, reverseMap, weightMap);
    adder.addBoth = true;
    for (int i = 0; i < m; i++) {
        int u, v, c, l;
        std::cin >> u >> v >> c >> l;
//        std::cout << "Adding edge " << u << " " << v << " " << c << " " << l;
        adder.addEdge(u, v, c, l);
    }


    // do a dijkstra
    std::vector<Vertex> p(num_vertices(G));
    std::vector<int> d(num_vertices(G));

//    std::vector<int> minDistance(n);
//    std::vector<int> minFlow(n);
//    DijkstraVisitor myV(capacityMap, minDistance, minFlow);

    dijkstra_shortest_paths(G, s,
                            predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, G))).
                                    distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, G))));

    Graph FG(n);
    EdgeCapacityMap capacityMap2 = get(edge_capacity, FG);
    EdgeReverseMap  reverseMap2 = get(edge_reverse, FG);
    EdgeResidualMap residualMap2 = get(edge_residual_capacity, FG);
    EdgeWeightMap weightMap2 = get(edge_weight, FG);
    EdgeAdder adder2(FG, capacityMap2, reverseMap2, weightMap2);

//    std::cout << s << " " << f << std::endl;
    EdgeIt e, eend;
    for (tie(e,eend) = edges(G); e != eend; e++) {
        Vertex u = source(*e, G);
        Vertex v = target(*e, G);
        if (weightMap[*e] == (d[v] - d[u])) { // the edge lies on this side
//            std::cout << u << " --> " << v << " (" << capacityMap[*e] << "), ";
            adder2.addEdge(u, v, capacityMap[*e], weightMap[*e]);
        }
    }
//    int marathonDistance = d[f] - d[s];
//
//    std::cout << "Shortest path " << marathonDistance << std::endl;

// Other idea: use visitor


    // find all the distances
//    for (int i = 0; i < n; i++) {
//        if (i != f && d[i] >= marathonDistance) {
//            // Clear if further away...
//            std::cout << "Will clear " << i << " cause " << d[i] << " is bigger than " << marathonDistance << std::endl;
//            clear_vertex(i, G);
//        }
//    }

    // workaround: Single shortest path
//    Graph FG(n);
//    EdgeCapacityMap capacityMap2 = get(edge_capacity, FG);
//    EdgeReverseMap  reverseMap2 = get(edge_reverse, FG);
//    EdgeResidualMap residualMap2 = get(edge_residual_capacity, FG);
//    EdgeWeightMap weightMap2 = get(edge_weight, FG);
//    EdgeAdder adder2(FG, capacityMap2, reverseMap2, weightMap2);
//    Vertex v = f;
//    while (v != s) {
//        Vertex parent = p[v];
//        Edge e; bool s;
//        boost::tie(e, s) = edge(parent, v, G);
//        adder2.addEdge(parent, v, capacityMap[e], weightMap[e]);
////        std::cout << v << " <-- " << parent << "(" << capacityMap[e] << ") ";
//        v = p[v];
//    }
//std::cout << std::endl;
    // Now we can max flow
    int flow = boost::push_relabel_max_flow(FG, s, f);
    std::cout << flow << std::endl;
}

int main() {
    int t; std::cin >> t;
//    t = 5;
    while(t--) {
        testcase();
    }
    return 0;
}