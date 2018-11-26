//
// Created by Hidde Lycklama on 11/5/18.
//

#include <iostream>
#include <string>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
                boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;
void addEdge(int from, int to, long capacity, Graph &G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap) {
    Edge e, rev_e;
    bool success;
    boost::tie(e, success) = boost::add_edge(from, to, G);
    boost::tie(rev_e, success) = boost::add_edge(to, from, G);
    capacitymap[e] = capacity;
    capacitymap[rev_e] = 0; // reverse edge has no capacity!
    revedgemap[e] = rev_e;
    revedgemap[rev_e] = e;
}


void testcase() {
    int n, m, s;
    std::cin >> n >> m >> s;

    std::vector<int> endCapacities(n);

    Graph G(n + 1);
    EdgeCapacityMap capacityMap = boost::get(boost::edge_capacity, G);
    ResidualCapacityMap residualMap = boost::get(boost::edge_residual_capacity, G);
    ReverseEdgeMap revEdgeMap = boost::get(boost::edge_reverse, G);

    Vertex start = 0;
    Vertex target = boost::add_vertex(G);

    for (int i = 0; i < s; i++ ){
        int shop; std::cin >> shop;
//        targets[shop] = 1;
            int cap = 1;
            endCapacities[shop] = cap;
        addEdge(shop, target, cap, G, capacityMap, revEdgeMap);
//        endCapacities[shop] = 1;
//        std::cout << "Adding edge " << shop << " - " << target << " with cap. " << cap << std::endl;
    }

    for (int i = 0; i < m; i++) {
        int v, u;
        std::cin >> v >> u;

        Edge e, rev_e;
        bool success;
        boost::tie(e, success) = boost::add_edge(v, u, G);
        boost::tie(rev_e, success) = boost::add_edge(u, v, G);
        capacityMap[e] = 1;
        capacityMap[rev_e] = 1; // reverse edge has no capacity!
        revEdgeMap[e] = rev_e;
        revEdgeMap[rev_e] = e;
//        std::cout << "Adding edge " << v << " - " << u << " with cap. " << 1 << std::endl;
    }

    EdgeIt ebeg, eend;
//    std::cout << "Source: " << s << std::endl;
////    std::cout << "Target: " << t << std::endl;
//    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
//        std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G)
//                  << " cap. " << capacityMap[*ebeg] << std::endl;
//    }

//    std::cout << "Flow" << std::endl;
    long flow1 = boost::push_relabel_max_flow(G, start, target);
//    long flow1 = boost::edmonds_karp_max_flow(G, s, t);
    bool found = true;
//    EdgeIt ebeg, eend;
//    std::cout << "Source: " << s << std::endl;
//    std::cout << "Target: " << t << std::endl;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
//        std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G)
//                  << " runs " << capacityMap[*ebeg] - residualMap[*ebeg]
//                  << " units of flow (negative for reverse direction)." << std::endl;
//        std::cout << capacityMap[*ebeg] << " - " << residualMap[*ebeg] << std::endl;
        Vertex vS = boost::source(*ebeg, G);
        Vertex vT = boost::target(*ebeg, G);
        long flow = capacityMap[*ebeg] - residualMap[*ebeg];
        if (vS == target) {
            if (endCapacities[vT] != -flow) {
                found = false;
                break;
            }
        }
    }
    if (found) {
        std::cout << "yes" << std::endl;
    } else {
        std::cout << "no" << std::endl;
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}