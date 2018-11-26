//
// Created by Hidde Lycklama on 11/2/18.
//

// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
// using namespace boost;


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
    int l, p;
    std::cin >> l >> p;

    Graph G(l);
    EdgeCapacityMap capacityMap = boost::get(boost::edge_capacity, G);
    ResidualCapacityMap residualMap = boost::get(boost::edge_residual_capacity, G);
    ReverseEdgeMap revEdgeMap = boost::get(boost::edge_reverse, G);

    Vertex s = l;
    Vertex t = l + 1;

    std::vector<std::vector<int>> locationRequirements(l, std::vector<int>(2)); // required num of soldiers (outflow)
    for (int i = 0; i < l; i++) {
        int flowIn, flowReq; std::cin >> flowIn >> flowReq;
        locationRequirements[i][0] = flowIn;
        locationRequirements[i][1] = flowReq;
    }
    for (int i = 0; i < p; i++) {
        int v, u, min, max;
        std::cin >> v >> u >> min >> max;
        addEdge(v, u, max - min, G, capacityMap, revEdgeMap);

        locationRequirements[u][0] += min;
        locationRequirements[v][1] += min;
    }

    for (int i = 0; i < l; i++) {
        int flowIn = locationRequirements[i][0];
        int flowReq = locationRequirements[i][1];
        addEdge(s, i, flowIn, G, capacityMap, revEdgeMap);
        addEdge(i, t, flowReq, G, capacityMap, revEdgeMap);
    }

    long flow1 = boost::push_relabel_max_flow(G, s, t);

    bool found = true;
    EdgeIt ebeg, eend;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
        Vertex source = boost::source(*ebeg, G);
        Vertex target = boost::target(*ebeg, G);
        int flow = capacityMap[*ebeg] - residualMap[*ebeg];
        if (source == t) {
            if (locationRequirements[target][1] > -flow) {
                std::cout << "no" << std::endl;
                found = false;
                break;
            }
        }
    }
    if (found) {
        std::cout << "yes" << std::endl;
    }
}

int main() {
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
}