//
// Created by Hidde Lycklama on 11/2/18.
//

// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <map>
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

void testcase(int tt) {
    int n, m;
    std::cin >> n >> m;

//    std::cout << tt << " " << n << " " << m << std::endl;

    Graph G(n + (m * 3));
    EdgeCapacityMap capacityMap = boost::get(boost::edge_capacity, G);
    ResidualCapacityMap residualMap = boost::get(boost::edge_residual_capacity, G);
    ReverseEdgeMap revEdgeMap = boost::get(boost::edge_reverse, G);

    Vertex s = n;
    Vertex t = n + 1;

    std::vector<int> playerScores(n);
    std::map<std::pair<int, int>, int> games;
//    std::vector<std::vector<int>> locationRequirements; // required num of soldiers (outflow)
    for (int i = 0; i < m; i++) {
        int p1, p2, res; std::cin >> p1 >> p2 >> res;
        if (res == 1) {
            playerScores[p1]++;
        } else if (res == 2) {
            playerScores[p2]++;
        } else if (res == 0) {
            //
            if (p1 < p2) { // distinct ordering
                games[std::pair<int, int>(p1, p2)]++;
            } else {
                games[std::pair<int, int>(p2, p1)]++;
            }
        }
    }
    std::vector<int> playerTargets(n);
    for (int i = 0; i < n; i++) {
        int t; std::cin >> t;
        playerTargets[i] = t;
    }

//    if (tt != 147) {
//        return;
//    }

    for (int i = 0; i < n; i++) {

        if (playerScores[i] != 0) {
            // Add from s to p
            addEdge(s, i, playerScores[i], G, capacityMap, revEdgeMap);
        }

        if (playerTargets[i] != 0) {
            // Add from p to t
            addEdge(i, t, playerTargets[i], G, capacityMap, revEdgeMap);
        }

//        std::cout << i << " " << playerScores[i] << std::endl;
    }

    std::vector<int> matchFlows(n + (m * 3));
    int cc = n + 2;
    for (auto iter = games.cbegin(); iter != games.cend(); iter++) {
//        std::cout << iter->first.first << " " << iter->first.second << " " << iter->second << std::endl;
        Vertex index = cc;

//        std::cout << "Adding edge from (source) " << s << " to " << index << " " << iter->second << std::endl;
//        std::cout << "Adding edge from " << index << " to " << iter->first.first <<std::endl;
//        std::cout << "Adding edge from " << index << " to " << iter->first.second <<std::endl;

        matchFlows[index] = iter->second;
        addEdge(s, index, iter->second, G, capacityMap, revEdgeMap);
        addEdge(index, iter->first.first, iter->second, G, capacityMap, revEdgeMap);
        addEdge(index, iter->first.second, iter->second, G, capacityMap, revEdgeMap);
        cc++;
    }

    EdgeIt ebeg, eend;
//    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
//        std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G) << std::endl;
//        std::cout
//                  << " capacity " << capacityMap[*ebeg] << std::endl;
//    }

    long flow1 = boost::push_relabel_max_flow(G, s, t);
//    long flow1 = boost::edmonds_karp_max_flow(G, s t)
    // check if each outflow matches
//    std::cout << flow1 << std::endl;

    bool found = true;
//    std::cout << "Source: " << s << std::endl;
//    std::cout << "Target: " << t << std::endl;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
//        std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G)
//                  << " runs " << capacityMap[*ebeg] - residualMap[*ebeg]
//                  << " units of flow (negative for reverse direction)." << std::endl;
//        std::cout << capacityMap[*ebeg] << " - " << residualMap[*ebeg] << std::endl;
        Vertex source = boost::source(*ebeg, G);
        Vertex target = boost::target(*ebeg, G);
        long flow = capacityMap[*ebeg] - residualMap[*ebeg];
        if (source == s && target > t) {
            // check flows to 'match' nodes
            if (matchFlows[target] != flow) {
                std::cout << "no" << std::endl;
                found = false;
                break;
            }
        }
        if (source == s && target < n) {
            // check initial flows match
            if (playerScores[target] != flow) {
//                std::cout << "hit" << std::endl;
                std::cout << "no" << std::endl;
                found = false;
                break;
            }
        }
        if (source == t) {
//            std::cout << playerTargets[target] << " " << flow << std::endl;
            if (playerTargets[target] != -flow) {
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
        testcase(t);
    }
    return 0;
}