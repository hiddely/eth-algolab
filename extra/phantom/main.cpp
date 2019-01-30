// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

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
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

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
        capacitymap[rev_e] = 0;
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};

// Main
void testcase() {
    // build Graph

    int n, m, s, d; std::cin >> n >> m >> s >> d;

    const int N = n - s - d + 2;
    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    Vertex src = 0;
    Vertex sink = 1;

    std::vector<std::pair<int, int> > edges(m);
    for (int i = 0; i < m; i++) {
        int u, v; std::cin >> u >> v;
        edges[i] = std::pair<int, int>(u, v);
    }

    std::vector<bool> starts(n);
    std::vector<bool> ends(n);
    for (int i = 0; i < s; i++) {
        int x; std::cin >> x;
        starts[x] = true;
    }
    for (int i = 0; i < d; i++) {
        int x; std::cin >> x;
        ends[x] = true;
    }
    for (int i = 0; i < m; i++) {
        int u = edges[i].first;
        int v = edges[i].second;
        if (starts[u]) {
            u = 0;
        }
        if (ends[v]) {
            v = 1;
        }
        eaG.addEdge(u, v, INT_MAX);
    }

    // Find a min cut via maxflow
    int flow = boost::push_relabel_max_flow(G, src, sink);
    std::cout << "maximum flow = minimum cut = " << flow << std::endl;

    // BFS to find vertex set S
    std::vector<int> vis(N, false); // visited flags
    std::queue<int> Q; // BFS queue (from std:: not boost::)
    vis[src] = true; // Mark the source as visited
    Q.push(src);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        OutEdgeIt ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = boost::target(*ebeg, G);
            // Only follow edges with spare capacity
            if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
            vis[v] = true;
            Q.push(v);
        }
    }

    // Output S
    for (int i = 0; i < N; ++i) {
        if (vis[i]) std::cout << i << " ";
    }
    std::cout << std::endl;


}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}