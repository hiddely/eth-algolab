//
// Created by Hidde Lycklama on 1/5/19.
//

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

namespace boost {
    enum edge_inclusion_t {
        edge_inclusion
    };
    BOOST_INSTALL_PROPERTY(edge, inclusion);
}
// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        boost::no_property,				// interior properties of vertices
        boost::property<boost::edge_weight_t, int,
                boost::property<boost::edge_inclusion_t, bool> > 		// interior properties of edges
>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t
typedef boost::property_map<Graph, boost::edge_inclusion_t>::type InclusionMap;

WeightMap weightMap;
Graph G;

struct Via {
    Edge edge;
    Vertex from;

    Via(Edge e, Vertex v): edge(e), from(v) {};
    Via() = default;

    bool operator < (const Via &v) const {
        if (weightMap[this->edge] == weightMap[v.edge]) {
            if (this->otherVertex() == v.otherVertex()) {
                return this->from < v.from;
            }
            return this->otherVertex() < v.otherVertex();
        }
        return weightMap[this->edge] > weightMap[v.edge];
    }

    Vertex otherVertex() const {
        Vertex a = boost::target(edge, G);
        Vertex b = boost::source(edge, G);
        if (a == from) {
            return b;
        } else if (b == from ){
            return a;
        } else {
            std::cerr << "This cant happen " << std::endl;
            std::cerr << "Debug: " << a << " " << b << " " << from << std::endl;
            abort();
        }
    }
};

void testcase() {
    int numNodes, startIndex;
    std::cin >> numNodes >> startIndex;

    G = Graph(numNodes);	// creates an empty graph on n vertices
    weightMap = boost::get(boost::edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38

    InclusionMap inclusionMap = boost::get(boost::edge_inclusion, G);

//    Edge edgeMap[numNodes][numNodes];

    for (int j = 1; j < numNodes; j++) {
        for (int k = 1; k < numNodes - j + 1; k++) {
            int w; std::cin >> w;
            Edge e; bool s;
            boost::tie(e, s) = boost::add_edge(j - 1, j + k - 1, G);
            weightMap[e] = w;
//            std::cerr << "Adding " << j - 1 << " -> " << j + k - 1 << ": " << w << std::endl;
        }
    }

//    std::vector<Via> storage()
    std::priority_queue<Via , std::vector<Via >> queue;
    std::vector<bool> visited(numNodes, false);

    Vertex start = startIndex - 1;

    OutEdgeIt e, eend;
    for (boost::tie(e, eend) = boost::out_edges(start, G); e != eend; e++) {
//        std::cerr << "Pushing " << *e << std::endl;
        queue.push(Via(*e, start));
    }
//    visited[start] = true;

    std::vector<std::vector<int>> maxWeightOnPath(numNodes);
    for(int i=0; i<numNodes; ++i)
    {
        maxWeightOnPath[i] = std::vector<int>(numNodes-i, 0);
    }
    std::vector<Vertex> added_vertices;
    while (!queue.empty()) {
        Via via = queue.top();
        queue.pop();
        Vertex t = via.otherVertex();
        if (visited[t]) {
            continue;
        }
        inclusionMap[via.edge] = true;
//        ma
//        std::cerr << "Included: " << via.edge << " " << std::endl;

        visited[t] = true;
//        std::cerr << "Visiting " << t << std::endl;
        OutEdgeIt e, eend;
        for (boost::tie(e, eend) = boost::out_edges(t, G); e != eend; e++) {
            Via n = Via(*e, t);
            if (!visited[n.otherVertex()]) {
                queue.push(n);
            }
        }

        Vertex source = via.from;
        for(auto vertex_it = added_vertices.begin(); vertex_it != added_vertices.end(); ++vertex_it) //ONein
        {
            maxWeightOnPath[std::min(*vertex_it, t)][std::max(*vertex_it, t) - std::min(*vertex_it, t)] =
                    std::max(weightMap[via.edge], maxWeightOnPath[std::min(source, *vertex_it)][std::max(source, *vertex_it) - std::min(source, *vertex_it)]);
//            std::cerr << "Setting " << t << "," << *vertex_it << ": max( " << weightMap[via.edge] << ", " <<  maxWeightOnPath[std::min(source, *vertex_it)][std::max(source, *vertex_it) - std::min(source, *vertex_it)] << std::endl;
//            std::cerr << "Source " << source << std::endl;
        }
        added_vertices.push_back(t);
    }

    EdgeIt a, aend;
    int minEdgeW = INT_MAX;
    long cost = 0;
    for (boost::tie(a, aend) = boost::edges(G); a != aend; a++) {
        if (!inclusionMap[*a]) {
            Vertex s = boost::source(*a, G);
            Vertex t = boost::target(*a, G);
            int calc = weightMap[*a] - maxWeightOnPath[std::min(s, t)][std::max(s, t)-std::min(s,t)];
//            std::cerr << s << ", " << t << ". " << weightMap[*a] << " - " << maxWeightOnPath[std::min(s, t)][std::max(s, t)-std::min(s,t)] << std::endl;
            minEdgeW = std::min(minEdgeW, calc) ;
        } else {
            cost += weightMap[*a];
        }
    }

    std::cout << cost + minEdgeW << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}