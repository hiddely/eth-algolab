//
// Created by Hidde Lycklama on 12/3/18.
//

#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
#include <set>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        boost::property<boost::vertex_index2_t, int>,				// interior properties of vertices
        boost::property<boost::edge_weight_t, int> 		// interior properties of edges
>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t
typedef boost::property_map<Graph, boost::vertex_index2_t>::type TempMap;

typedef std::pair<int, int> MinMax;

MinMax find(std::deque<Vertex> &window, TempMap &temp) {
    int min = INT_MAX, max = INT_MIN;
    for (auto iter = window.begin(); iter != window.end(); iter++) {
        min = std::min(min, temp[*iter]);
        max = std::max(max, temp[*iter]);
    }
    return std::pair<int, int>(min, max);
}

void testcase() {
    int n, m, k;
    std::cin >> n >> m >> k;

    Graph G(n);
//    WeightMap weightmap = boost::get(boost::edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38
//    TempMap tempMap = boost::get(boost::vertex_index2, G);

    std::vector<int> temps(n);
    for (int i = 0; i < n; i ++) {
        int temp; std::cin >> temp;
        temps[i] = temp;
    }

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        Edge e; bool s;
        boost::tie(e, s) = boost::add_edge(u, v, G);
    }

    std::queue<Vertex> toVisit;
    std::multiset<int> vals;
    Vertex start = 0;
    toVisit.push(start);
    while(!toVisit.empty()) {
        Vertex c = toVisit.front();
        toVisit.pop();

        OutEdgeIt e, eend;
        for (boost::tie(e, eend) = boost::out_edges(c, G); e != eend; e++) {
            // out edges
            Vertex n = boost::target(*e, G);
            toVisit.push(n);
        }
    }

    std::vector<int> starts;
    std::vector<bool> reached(n, false); // optimization for tree structure
    std::vector<int> minMap(n, -1);
    std::vector<int> maxMap(n, -1);
    for (int i = 0; i < n; i++) {
        if (leafs[i]) {
            int current = i;
            bool reachedTop = false;
            while (current != 0 && !reachedTop && !reached[current]) {
                // traverse to top
                reached[current] = true;
                int min = temps[current];
                int max = temps[current];
                int parent = current;
                bool cont = false;
                for (int j = 0; j < m - 1; j++) {
                    parent = predmap[parent];

                    min = std::min(min, temps[parent]);
                    max = std::max(max, temps[parent]);
                    if (max - min > k) {
                        cont = true;
                        break;
                    }
                    if (j < m - 2 && parent == 0) {
                        //
                        reachedTop = true;
                        break;
                    }
                }
                minMap[current] = min;
                maxMap[current] = max;
                if (cont) {
                    current = predmap[current];
                    continue;
                }
                if (reachedTop) {
                    break;
                }
                // else, see if match
                int diff = max - min;
                if (diff <= k) {
//                    std::cerr << max << " - " << min << " is alright. Pushing " << parent << ", from: " << current << std::endl;
                    starts.push_back(parent);
                }
                current = predmap[current];
            }

        }
    }

    std::sort(starts.begin(), starts.end());
    std::set<int> s;
    unsigned size = starts.size();
    for( unsigned i = 0; i < size; ++i ) s.insert( starts[i] );
    starts.assign(s.begin(), s.end());

    if (starts.empty()) {
        std::cout << "Abort mission" << std::endl;
    } else {
        for (auto iter = starts.cbegin(); iter != starts.cend(); iter++) {
            std::cout << *iter << " ";
        }
        std::cout <<std::endl;
    }

    // start at 0
//    Vertex next = 0;
//    std::deque<Vertex> window;
////    window.push_back(next);
//    std::set<int> starts;
//    int windowSize = 0;
//    while (next != -1) {
////        std::cout << " -> " << next << "(" << tempMap[next] << ")";
//        window.push_back(next);
//        windowSize++;
//        if (windowSize > m) { // remove to optimize
//            window.pop_front();
//            windowSize--;
//        }
//        if (windowSize == m) {
//            MinMax mm = find(window, tempMap);
//
////            std::cout << "Min " << min->temp << ", max " << max->temp << ", size " << size << std::endl;
//
//            if (abs(mm.first - mm.second) <= k) {
//                // we have a match
////                std::cout << max->temp - min->temp << " <= " << k << std::endl;
////                std::cout << "ADding " << next << std::endl;
//                starts.insert(window.front());
//            }
//        }
//
//        OutEdgeIt iter, iterEnd;
//        boost::tie(iter, iterEnd) = boost::out_edges(next, G);
//        if (iter != iterEnd) {
//            next = boost::target(*iter,G);
//        } else {
////            std::cout <<" No out edges found for " << next << std::endl;
//            break;
//        }
//    }



}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) {
        testcase();
    }
    return 0;
}