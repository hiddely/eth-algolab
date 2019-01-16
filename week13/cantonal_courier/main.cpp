//
// Created by Hidde Lycklama on 1/10/19.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
// Includes
// ========
// STL includes
#include <algorithm>
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


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap	&capacitymap;
    ReverseEdgeMap	&revedgemap;

public:
    // to initialize the Object
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
            G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

    // to use the Function (add an edge)
    void addEdge(int from, int to, long capacity) {
        Edge e, rev_e;
        bool success;
        boost::tie(e, success) = boost::add_edge(from, to, G);
        boost::tie(rev_e, success) = boost::add_edge(to, from, G);
        capacitymap[e] = capacity;
        capacitymap[rev_e] = 0; // reverse edge has no capacity!
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};



void testcase() {
    int numZones, numJobs;
    std::cin >> numZones >> numJobs;

    Graph G(numZones + numJobs + 2);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    const int OFFSET_ZONE = 0;
    const int OFFSET_JOBS = numZones;
    const int START = numZones + numJobs;
    const int END = START + 1;

    for (int i = 0; i < numZones; i++) {
        int c; std::cin >> c;

        eaG.addEdge(OFFSET_ZONE + i, END, c);
    }
    long totalProfit = 0;
    for (int i = 0; i < numJobs; i++) {
        int r; std::cin >> r;
        totalProfit += r;

        eaG.addEdge(START, OFFSET_JOBS + i, r);
    }

    long profit = 0;
    for (int i = 0; i < numJobs; i++) {
        int n; std::cin >> n;
        for (int j = 0; j < n; j++) {
            int z; std::cin >> z;
//            std::cout << i << " " << z << " true " << std::endl;
            eaG.addEdge(OFFSET_JOBS + i, OFFSET_ZONE + z, LONG_MAX);
        }
    }

    long flow1 = boost::push_relabel_max_flow(G, START, END);


    std::cout << totalProfit - flow1 << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}