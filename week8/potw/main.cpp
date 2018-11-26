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
    int h; int w;
    std::cin >> h >> w;
    std::string note;
    std::cin >> note;
    // convert to ints
    const int AOffset = 65;
    std::vector<int> noteOcc(26, 0);
    for(char& c : note) {
        int index = c - AOffset;
        noteOcc[index] += 1;
    }

    Graph G(28);
    EdgeCapacityMap capacityMap = boost::get(boost::edge_capacity, G);
    ResidualCapacityMap residualMap = boost::get(boost::edge_residual_capacity, G);
    ReverseEdgeMap revEdgeMap = boost::get(boost::edge_reverse, G);

    Vertex s = 0;
    Vertex t = 1;
//    Vertex

    for (int i = 0; i < 26; i++) {
        if (noteOcc[i] > 0) { // If there exists a letter, at edge with weight
            addEdge(i + 2, t, noteOcc[i], G, capacityMap, revEdgeMap);
        }
    }

    std::vector<std::vector<std::pair<int, int> > > newspaper (h, std::vector<std::pair<int, int> >(w));
    for (int i = 0; i < h; i++) {
        std::string line;
        std::cin >> line;
        int a = 0;
        for(char& c : line) {
            int index = c - AOffset;
            newspaper[i][a].first = index;
            a++;
        }
    }
    for (int i = 0; i < h; i++) {
        std::string line;
        std::cin >> line;
        int a = w - 1; // we decrement from the other side
        for(char& c : line) {
            int index = c - AOffset;
            newspaper[i][a].second = index;
            a--;
        }
    }

    std::vector<int> directFlow(26);

    // This is an optimization to pass the last test case, instead of adding an edge of 1 for each pair
    // we collect the pairs
    std::vector<std::vector<int> > insertMap(26, std::vector<int>(26));
    for (int i = 0; i < h; i++) {
        for (int a = 0; a < w; a++) {
            // set pairs
            int front = newspaper[i][a].first;
            int back = newspaper[i][a].second;

            if (noteOcc[front] != 0 && noteOcc[back] == 0) {
                directFlow[front]++;
            } else if(noteOcc[back] != 0 && noteOcc[front] == 0) {
                directFlow[back]++;
            } else {
//                Vertex v = boost::add_vertex(G);
//                // add from s
//                addEdge(s, v, 1, G, capacityMap, revEdgeMap);
//                addEdge(v, front + 2, 1, G, capacityMap, revEdgeMap);
//                addEdge(v, back + 2, 1, G, capacityMap, revEdgeMap);

                insertMap[front][back] += 1;
            }
        }
    }
    for (int a = 0; a < 26; a++) {
        for (int b = 0; b < 26; b++) {
            if (insertMap[a][b] != 0) {
                Vertex v = boost::add_vertex(G);
                // add from s
                addEdge(s, v, insertMap[a][b], G, capacityMap, revEdgeMap);
                addEdge(v, a + 2, insertMap[a][b], G, capacityMap, revEdgeMap);
                addEdge(v, b + 2, insertMap[a][b], G, capacityMap, revEdgeMap);
            }
        }
    }

    // optimize direct flow without vertex
    for (int i = 0; i < 26; i++) {
        if (directFlow[i] != 0) {
            addEdge(s, i + 2, directFlow[i], G, capacityMap, revEdgeMap);
        }
    }

//    std::cout << "Flow" << std::endl;
    long flow1 = boost::push_relabel_max_flow(G, s, t);
//    long flow1 = boost::edmonds_karp_max_flow(G, s, t);
    bool found = true;
    EdgeIt ebeg, eend;
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
        if (source == t) {
            if (noteOcc[target - 2] != -flow) {
                found = false;
                break;
            }
        }
    }
    if (found) {
        std::cout << "Yes" << std::endl;
    } else {
        std::cout << "No" << std::endl;
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