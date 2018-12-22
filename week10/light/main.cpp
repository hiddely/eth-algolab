//
// Created by Hidde Lycklama on 11/24/18.
//

#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Circle_2 Circle;

#include <CGAL/Simple_cartesian.h>
typedef CGAL::Simple_cartesian<double> Rep;
#include <CGAL/Apollonius_graph_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Apollonius_graph_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Apollonius_graph_filtered_traits_2.h>
// typedef for the traits; the filtered traits class is used
typedef CGAL::Apollonius_graph_filtered_traits_2<Rep> Traits;
// typedefs for the algorithm
// With the second template argument in the vertex base class being
// false, we indicate that there is no need to store the hidden sites.
// One case where this is indeed not needed is when we only do
// insertions, like in the main program below.
typedef CGAL::Apollonius_graph_vertex_base_2<Traits,false>   Vb;
typedef CGAL::Triangulation_face_base_2<Traits>              Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>       Agds;
typedef CGAL::Apollonius_graph_2<Traits,Agds>    Apollonius_graph;

void testcase() {
    int m, n; std::cin >> m >> n;

    std::vector<Circle> people(m);
    std::vector<bool> alive(m, true);
    for (int i = 0; i < m; i ++) {
        Point center;
        std::cin >> center;
        long radius; std::cin >> radius;
        people[i] = Circle(center, radius * radius);
//        std::cout << people[i] << std::endl;
    }

    Apollonius_graph ag;
    ag.in

    Triangulation t;

    int h; std::cin >> h;
    double radius = h*h / 4.0;
    std::vector<Circle> lights(n);
    for (int i = 0; i < n; i++) {
        Point center;
        std::cin >> center;
        Circle light = Circle(center, radius);
        lights[i] = light;
//        for (int a = 0; a < m; a++) {
//            if (alive[a]) {
//                // Test
//                bool res = CGAL::do_intersect(light, people[a]);
////                std::cout << "Testing " << light << " with " << people[a] << " " << res << std::endl;
//                if (res) {
//                    alive[a] = false;
//                    // Lost!
//                }
//            }
//        }
    }

    for (int i = 0; i < m; i++) {
        bool ded = false;
        for (int a = 0; a < n; a++) {
            if (CGAL::do_intersect(people[i], lights[a])) {
                ded = true;
                break;
            }
        }
        if (!ded) {
            std::cout << i << " ";
        }
//        if (alive[i]) {
//            std::cout << i << " ";
//        }
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