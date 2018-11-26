//
// Created by Hidde Lycklama on 11/15/18.
//

#include <iostream>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <set>

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
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &edgeCapacityMap;
    ReverseEdgeMap &reverseEdgeMap;

public:
    EdgeAdder(Graph &gr, EdgeCapacityMap &ecm, ReverseEdgeMap &rem): G(gr), edgeCapacityMap(ecm), reverseEdgeMap(rem) {}

    void addEdge(int u, int v, int w) {
        Edge e; bool s;
        Edge rev_e; bool rev_s;
        boost::tie(e, s) = boost::add_edge(u, v, G);
        boost::tie(rev_e, rev_s) = boost::add_edge(v, u, G);
        edgeCapacityMap[e] = w;
        edgeCapacityMap[rev_e] = 0;
        reverseEdgeMap[e] = rev_e;
        reverseEdgeMap[rev_e] = e;
    }
};

void testcase() {
    int g, s, l;
    std::cin >> g >> s >> l;

//    std::cout << g << " " << s << " " << l << std::endl;

    Graph G(g + s + 2);
    EdgeCapacityMap capacityMap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revEdgeMap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap residualCapacityMap = boost::get(boost::edge_residual_capacity, G);

    EdgeAdder adder(G, capacityMap, revEdgeMap);

    Vertex source = g + s;
    Vertex target = g + s + 1;

    for (int i = 0; i < g; i++) {
        adder.addEdge(source, i, 1);
    }
    for (int i = 0; i < s; i++) {
        adder.addEdge(i + g, target, 1);
    }

    for (int i = 0; i < l; i++) {
        int u, v; // u = g, v = s
        std::cin >> u >> v;
        adder.addEdge(u, v + g, 1);
    }

    int flow = boost::push_relabel_max_flow(G, source, target);
//    std::cout << "Flow " << flow << std::endl;

    std::vector<bool> visited(g + s + 2, false);
    std::queue<Vertex> Q;
    Q.push(source);
    visited[source] = true;
    while (!Q.empty()) {
        //
        Vertex u = Q.front();
        Q.pop();

//        std::cout << "Visiting " << u << " ";

        OutEdgeIt ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ebeg++) {
            Vertex otherEnd = boost::target(*ebeg, G);
//            std::cout << " edge to " << otherEnd << " " << residualCapacityMap[*ebeg];
            if (!visited[otherEnd] && residualCapacityMap[*ebeg] != 0) {
//                std::cout << " Adding " << otherEnd;
                visited[otherEnd] = true;
                Q.push(otherEnd);
            }
        }
//        std::cout << std::endl;
    }

    std::set<int> grounds;
    for (int i = 0; i < g; i++) {
        if (!visited[i]) {
            grounds.insert(i);
        }
    }
    std::set<int> sats;
    for (int i = 0; i < s; i++) {
        if (visited[g + i]) {
            sats.insert(g + i);
        }
    }
    std::cout << grounds.size() << " " << sats.size() << std::endl;
//    bool first = true;
    for (auto iter = grounds.cbegin(); iter != grounds.cend(); iter++) {
//        if (!first) {
//            std::cout << " ";
//        } else {
//            first = false;
//        }
        std::cout << *iter << " ";
    }
    for (auto iter = sats.cbegin(); iter != sats.cend(); iter++) {
        std::cout << (*iter - g) << " ";
    }
    std::cout << std::endl;
}

int main() {
    int t; std::cin >> t;
    while(t--) {
        testcase();
    }
    return 0;
}