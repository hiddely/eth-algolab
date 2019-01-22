//
// Created by Hidde Lycklama on 10/17/18.
//

//
// Created by Hidde Lycklama on 10/17/18.
//

#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <unordered_map>
#include <limits>
#include <stack>
#include <list>
#include <algorithm>

using namespace boost;

namespace boost
{
    struct edge_component_t
    {
        enum
        { num = 555 };
        typedef edge_property_tag kind;
    }
            edge_component;
}

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,	boost::no_property,				// interior properties of vertices
        boost::property< boost::edge_component_t, std::size_t > >					Graph;
//typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
//        boost::property<boost::edge_weight_t, int> > Graph;

typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::property_map<Graph, boost::edge_component_t>::type	ComponentMap;	// property map to access the interior property edge_weight_t
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::property_map<Graph, boost::vertex_index_t>::type VertexIndexMap;

void testcase() {
    int n, m;
    std::cin >> n >> m;

    Graph G(n);
    ComponentMap componentMap = boost::get(boost::edge_component, G);
    for (int i = 0; i < m; i++) {
        int v, u;
        std::cin >> v >> u;
        if (u < v) {
            std::swap(v, u);
        }
        Edge e; bool success;
        boost::tie(e, success) = boost::add_edge(v, u, G);
    }

    std::size_t num_comps = boost::biconnected_components(G, componentMap);

    std::map<int, int> componentCount;
    EdgeIt e, eend;
    for (boost::tie(e, eend) = boost::edges(G); e != eend; e++) {
        componentCount[componentMap[*e]]++;
    }

    int uniques = 0;
    for (auto iter = componentCount.begin(); iter != componentCount.end(); iter++) {
        if (iter->second == 1) {
            uniques++;
        }
    }

    std::vector<std::pair<int, int>> output;
    for (boost::tie(e, eend) = boost::edges(G); e != eend; e++) {
        if (componentCount[componentMap[*e]] == 1) {
            // add to list
            output.push_back(std::make_pair(boost::source(*e, G), boost::target(*e, G)));
        }
    }
    std::sort(output.begin(), output.end());

    std::cout << uniques << std::endl;
    for (int i = 0; i < output.size(); i++){
        std::cout << output[i].first << " " << output[i].second << std::endl;
    }
}

int main() {
    int t; std::cin >> t;
    while (t > 0) {
//        std::cout << t << std::endl;
        testcase();
        t--;
    }
}