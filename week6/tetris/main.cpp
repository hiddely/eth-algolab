//
// Created by Hidde Lycklama on 11/5/18.
//

#include <iostream>
#include <vector>
#include <map>

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
    int w, n;
    std::cin >> w >> n;

    Graph G(w * 2);
    EdgeCapacityMap capacityMap = boost::get(boost::edge_capacity, G);
    ResidualCapacityMap residualMap = boost::get(boost::edge_residual_capacity, G);
    ReverseEdgeMap revEdgeMap = boost::get(boost::edge_reverse, G);

    Vertex start = 0;
    Vertex target = 1;

    std::vector<std::vector<int> > edgeMap (w + 1, std::vector<int>(w + 1, 0));
    for (int i = 0 ; i < n; i++) {
        int u, v;
        std::cin >> u >> v;
        if (u > v) {
            edgeMap[v][u]++;
        } else {
            edgeMap[u][v]++;
        }
//        std::cout << "Edge " << u << " - " << v << std::endl;
//        addEdge(u, v, 1, G, capacityMap, revEdgeMap);
    }
    for (int i = 1; i < w; i++) {
        addEdge(2 * i + 1, 2 * i + 2, 1, G, capacityMap, revEdgeMap); // add connecting pair edges
    }
    for (int a = 0; a < w + 1; a++) {
        for (int b = 0; b < w + 1; b++) {
            if (edgeMap[a][b] > 0) {
                Vertex v1; Vertex v2;
                if (a == 0) {
                    v1 = 0;
                } else {
                    v1 = a * 2 + 2; // + 2 cause we wlil be sending from here
                }
                if (b == w) {
                    v2 = 1;
                } else {
                    v2 = b * 2 + 1;
                }
                addEdge(v1, v2, edgeMap[a][b], G, capacityMap, revEdgeMap);
            }
        }
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

    std::cout << flow1<< std::endl;
//    long flow1 = boost::edmonds_karp_max_flow(G, s, t);
//    bool found = true;
//    EdgeIt ebeg, eend;
//    std::cout << "Source: " << s << std::endl;
//    std::cout << "Target: " << t << std::endl;
//    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
//        std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G)
//                  << " runs " << capacityMap[*ebeg] - residualMap[*ebeg]
//                  << " units of flow (negative for reverse direction)." << std::endl;
////        std::cout << capacityMap[*ebeg] << " - " << residualMap[*ebeg] << std::endl;
//
//    }
//    if (found) {
//        std::cout << "yes" << std::endl;
//    } else {
//        std::cout << "no" << std::endl;
//    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}