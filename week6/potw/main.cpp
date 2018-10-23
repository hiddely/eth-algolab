
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <climits>

using namespace boost;

typedef property<edge_weight_t, int> EdgeProperty;
typedef boost::adjacency_list<vecS, vecS, directedS, no_property, EdgeProperty> Graph;

typedef property_map<Graph, edge_weight_t>::type WeightMap;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;

void testcase() {
    int n, m, k, T;
    std::cin >> n >> m >> k >> T;

    Graph G(n);
    WeightMap weights = get(boost::edge_weight, G);
    std::vector<int> teleports(T); // Vertex i teleports
    for (int i = 0; i < T; i++) {
        int n; std::cin >> n;
        teleports[i] = n;
    }
    for (int i = 0; i < m; i++) {
        int u, v, c;
        std::cin >> u >> v >> c;
        Edge e; bool s;
        boost::tie(e, s) = boost::add_edge(v, u, G); // Add from v to u!
        weights[e] = c;
    }


    std::vector<int> component(n), discover_time(n);
    std::vector<default_color_type> color(n);
    std::vector<Vertex> root(n);
    int num = strong_components(G, make_iterator_property_map(component.begin(), get(vertex_index, G)),
                                root_map(make_iterator_property_map(root.begin(), get(vertex_index, G))).
                                        color_map(make_iterator_property_map(color.begin(), get(vertex_index, G))).
                                        discover_time_map(make_iterator_property_map(discover_time.begin(), get(vertex_index, G))));

//    for (int i = 0; i != component.size(); ++i)
//        std::cout << "Vertex " << i
//                  <<" is in component " << component[i] << std::endl;

    std::vector<std::set<int> > filtered(num);
    for (int i = 0; i < T; i++) {
//        std::cout << "Adding " << i << " " << teleports[i] << " " << component[teleports[i]] << std::endl;
        filtered[component[teleports[i]]].insert(teleports[i]);
//        filtered[i] = component[teleports[i]];
    }

    // Count the weights of the components !
//    std::vector<int> ccount(T, -1);
//    for (int i = 0; i != filtered.size(); ++i) {
//        ccount[filtered[i]]++;
//    }

//    for (int i = 0; i != filtered.size(); ++i) {
//        std::cout << "C: " << ccount[filtered[i]] << std::endl;
//    }

//std::cout << "before edges " << T << std::endl;

    // Add the edges
    for (int i = 0; i < num; i++) {
        std::set<int> b = filtered[i];
        int size = b.size() - 1;
        Vertex v = boost::add_vertex(G);
        auto it1 = b.cbegin();
        for (; it1 != b.cend(); it1++) {
            Edge e; bool s;
//            std::cout << "Adding edge between " << v << " and " << *it1 << std::endl;
            boost::tie(e, s) = boost::add_edge(*it1, v, G);
            weights[e] = size;
            boost::tie(e, s) = boost::add_edge(v, *it1, G);
            weights[e] = 0;
        }
//            for (auto it2 = b.cbegin(); it2 != b.cend(); it2++) {
//                if (it1 == it2) {
//                    continue;
//                }
//                Edge e; bool s;
//                boost::tie(e, s) = boost::add_edge(*it2, *it1, G);
//                weights[e] = size;
//            }
//        }
//        for (int a = 0; a < T; a++) {
//            if (a == i) {
//                continue;
//            }
//            // check if strongly connected
//            if (filtered[i] == filtered[a]) {
//                Edge e; bool s;
//                boost::tie(e, s) = boost::add_edge(teleports[i], teleports[a], G);
//                weights[e] = ccount[filtered[i]];
////                std::cout << "Adding edge from " << teleports[i] << " to " << teleports[a] << std::endl;
//            }
//        }
    }

//    std::cout << "a2" << std::endl;


    // now we dijkstra
    std::vector<Vertex> p(n + num);
    std::vector<int> d(n + num);
    Vertex s = n - 1;

    dijkstra_shortest_paths(G, s,
                            predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, G))).
                                    distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, G))));

//    std::cout << "a3" << std::endl;

//    std::cout << "distances and parents:" << std::endl;
//    graph_traits < Graph >::vertex_iterator vi, vend;
//    for (boost::tie(vi, vend) = vertices(G); vi != vend; ++vi) {
//        std::cout << "distance(" << *vi << ") = " << d[*vi] << ", ";
//        std::cout << "parent(" << *vi << ") = " << p[*vi] << std::endl;
//    }
//    std::cout << std::endl;

    int min = INT_MAX;
    for (int i = 0; i < k; i++) {
//        std::cout << "Dist " << d[i] << std::endl;
        min = std::min(min, d[i]);
    }

    if (min > 1000000) {
        std::cout << "no" << std::endl;
    } else {
        std::cout << min << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t > 0) {
        testcase();
        t--;
    }
}