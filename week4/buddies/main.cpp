//
// Created by Hidde Lycklama on 10/18/18.
//

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <queue>
#include <map>

typedef std::string Char;
typedef std::set<Char> Chars;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,	boost::property<boost::vertex_name_t, Chars>,				// interior properties of vertices
boost::no_property >					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::property_map<Graph, boost::vertex_name_t>::type	CharMap;	// property map to access the interior property edge_weight_t

const Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();

struct CharO {
    Char c;
    int occ;

    CharO(Char ch, int occu): c(ch), occ(occu) {}

    bool operator < (const CharO &c2) const {
        return this->occ < c2.occ;
    }
};

void testcase() {
    int n, c, f;
    std::cin >> n >> c >> f;
    Graph G(n);
//    CharMap charMap = boost::get(boost::vertex_name, G);
    std::map<Char, std::set<int>> vertexes;
    for (int i = 0; i < n; i++) {
        for (int a = 0; a < c; a++) {
            Char c; std::cin >> c;
            vertexes[c].insert(i);
        }
    }
    std::map<Char, std::set<int>>::iterator it;
    std::vector<std::vector<int>> thresholdMap(n, std::vector<int>(n, f));
    for ( it = vertexes.begin(); it != vertexes.end(); it++ )
    {
        Char key = it->first;
        std::set<int> vertices = it->second;
        auto s1 = vertices.cbegin();
        for (; s1 != vertices.cend(); s1++) {
            auto s2 = vertices.cbegin();
            for (; s2 != vertices.cend(); s2++) {
                if (thresholdMap[*s1][*s2] > 0) {
                    thresholdMap[*s1][*s2]--;
                    continue;
                }
                Edge e; bool success;
                boost::tie(e, success) = boost::add_edge(*s1, *s2, G);
//                std::cout << "Adding " << *s1 << " " << *s2 << std::endl;
            }
        }
    }
    std::vector<Vertex> mate(n);
    edmonds_maximum_cardinality_matching(G, &mate[0]);

    int count = 0;
    for(int i = 0; i < n; ++i) {
        if (mate[i] != NULL_VERTEX && i < mate[i]) {
//                        std::cout << i << " -- " << mate[i] << std::endl;
            count++;
        }
    }

    if (count * 2 == n) {
        // There is a perfect matching, even when removing
        // since f = 0;
        std::cout << "not optimal" << std::endl;

//        if (f == 0) {
//        } else {
//            std::cout << "optimal" << std::endl;
//        }
    } else {
        // No perfect matching, f was optimal
        std::cout << "optimal" << std::endl;
//        if (f == 0) {
//        } else {
//            std::cout << "optimal" << std::endl;
//        }
    }

//    std::priority_queue<CharO> chars;
//    std::map<Char, int>::iterator it;
//    for ( it = occ.begin(); it != occ.end(); it++ )
//    {
//        chars.push(CharO(it->first, it->second));
//        std::cout << it->first << " " << it->second << std::endl;
//    }
//    std::vector<int> weights(n);
//    for (int i = 0; i < n; i++) {
//        auto it = students[i].cbegin();
//        for (; it != students[i].cend(); it++) {
//            weights[i] += occ[*it];
//        }
//        std::cout << "W " << weights[i];
//    }
    if (f == 0) {

    }
//    std::cout << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t > 0) {
        testcase();
        t--;
    }
}