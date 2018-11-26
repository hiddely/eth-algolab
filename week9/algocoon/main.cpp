//
// Created by Hidde Lycklama on 11/15/18.
//

#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                    boost::property<boost::edge_capacity_t, long,
                        boost::property<boost::edge_residual_capacity_t, long,
                            boost::property<boost::edge_reverse_t, Traits::edge_descriptor > > > > Graph;

typedef boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseEdgeMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type EdgeResidualMap;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;

class EdgeAdder {
    Graph &g;
    EdgeCapacityMap &capacityMap;
    ReverseEdgeMap &revMap;

public:
    EdgeAdder(Graph &a, EdgeCapacityMap &c, ReverseEdgeMap &d): g(a), capacityMap(c), revMap(d) {}

    void addEdge(int u, int v, int c) {
        Edge e, rev_e;
        bool s;
        boost::tie(e, s) = boost::add_edge(u, v, g);
        boost::tie(rev_e, s) = boost::add_edge(v, u, g);
        revMap[e] = rev_e;
        revMap[rev_e] = e;
        capacityMap[e] = c;
        capacityMap[rev_e] = 0;
    }
};
// TODO: Understand
void testcase() {
    int n, m;
    std::cin >> n >> m;

    Graph G(n);
    EdgeCapacityMap capacityMap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revEdgeMap = boost::get(boost::edge_reverse, G);
    EdgeResidualMap edgeResidualMap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder edgeAdder(G, capacityMap, revEdgeMap);

    Vertex source = 0;
    Vertex target = n - 1;

    for (int i = 0; i < m; i++) {
        int u, v, c;
        std::cin >> u >> v >> c;
//
        edgeAdder.addEdge(u, v, c);
    }

    int minFlow = INT_MAX;
    int minS, minT;
    for (int i = 1; i < n; i++) {
        // try all possible flows
        int flow1 = boost::push_relabel_max_flow(G, 0, i);
        if (flow1 < minFlow) {
            minFlow = flow1;
            minS = 0;
            minT = i;
        }
        int flow2 = boost::push_relabel_max_flow(G, i, 0);
        if (flow2 < minFlow) {
            minFlow = flow2;
            minS = i;
            minT = 0;
        }
    }

    boost::push_relabel_max_flow(G, minS, minT);

//    boost::successive_shortest_path_nonnegative_weights(G, source, target);

    std::cout << minFlow << std::endl;

    std::vector<bool> visited(n + 2, false);
    std::queue<Vertex> Q;
    Q.push(minS);
    visited[minS] = true;
    while (!Q.empty()) {
        //
        Vertex u = Q.front();
        Q.pop();

//        std::cout << "Visiting " << u << " ";

        OutEdgeIt ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ebeg++) {
            Vertex otherEnd = boost::target(*ebeg, G);
//            std::cout << " edge to " << otherEnd << " " << edgeResidualMap[*ebeg];
            if (!visited[otherEnd] && edgeResidualMap[*ebeg] != 0) {
//                std::cout << " Adding " << otherEnd;
                visited[otherEnd] = true;
                Q.push(otherEnd);
            }
        }
//        std::cout << std::endl;
    }


    std::set<int> chosen;
    for (int i = 0; i < n; i++) {
        if (visited[i]) {
            chosen.insert(i);
        }
    }

    std::cout << chosen.size() << " ";
    for (auto iter = chosen.cbegin(); iter != chosen.cend(); iter++) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}