//
// Created by Hidde Lycklama on 12/21/18.
//

// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <tuple>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Face_handle Face_handle;

// choose exact integral type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef K::Point_2 Point;
typedef std::tuple<int, int, int> Particle;

void testcase() {
    // by default, we have an LP with Ax >= b and no bounds for
    // the four variables alpha, beta, gamma, delta

    int a, s, b, e;
    std::cin >> a >> s >> b >> e;

    Program lp (CGAL::SMALLER, true, 0, true, e);

    const int OFFSET_E = a;

    std::vector<Particle> particles(a);
    std::vector<Point> lasers(s);
    std::vector<Point> bounty(b);

    for (int i = 0; i < a; i++) {
        // add constraints?
        int x, y, d;
        std::cin >> x >> y >> d;
        particles[i] = std::make_tuple(x, y, d);
//        lp.set_b(i, d); // TODO: x,y
    }
    for (int i = 0; i < s; i++) {
        // add vars
        int x, y;
        std::cin >> x >> y;
        lasers[i] = Point(x, y);

        lp.set_c(i, 1);
    }
    for (int i = 0; i < b; i++) {
        // add constraints?
        int x, y;
        std::cin >> x >> y;
        bounty[i] = Point(x, y);
    }

    std::vector<K::FT> maxRadius(s);
    if (b > 0) {
        Triangulation t;
        t.insert(bounty.begin(), bounty.end());

        for (int i = 0; i < s; i++) {
            K::FT minDist = CGAL::squared_distance(t.nearest_vertex(lasers[i])->point(), lasers[i]);
//        K::FT minDist = K::FT(INT_MAX);
            // minDist is the max radius...
            maxRadius[i] = minDist;
        }
    }


    for (int i = 0; i < a; i++) {
        lp.set_b(i, ET(std::get<2>(particles[i])));
        lp.set_r(i, CGAL::LARGER);
        int cx = std::get<0>(particles[i]);
        int cy = std::get<1>(particles[i]);
        Point p = Point(cx, cy);
        for (int x = 0; x < s; x++) {
            auto dist = CGAL::squared_distance(p, lasers[x]);
//            std::cerr << "Point " << p << ", " << lasers[x] << ". Dist " << 1 / std::max(dist.exact(), ET(1.0)) << std::endl;
            if (b == 0 || dist < maxRadius[x]) {
                lp.set_a(x, i, 1 / std::max(ET(dist), ET(1.0)));
            }
        }
    }
    Solution sol = CGAL::solve_linear_program(lp, ET());
    assert (sol.solves_linear_program(lp));

    if (sol.is_optimal() && CGAL::to_double(sol.objective_value()) <= e) {
        std::cout << "y" << std::endl;
//        std::cerr << CGAL::to_double(sol.objective_value()) << std::endl;
    } else {
        std::cout << "n" << std::endl;
    }
//    std::cout << sol << std::endl;

//    Solution::Variable_value_iterator opt = sol.variable_values_begin();
//    ET sw_ratio = opt->numerator() / opt->denominator();
//    ET cs_ratio = (opt+1)->numerator() / (opt+1)->denominator();

//    Triangulation t;
//    t.insert(lasers.begin(), lasers.end()); // n log n
//
//    for (int i = 0; i < a; i++) {
//        // find the nearest lasers
//        Face_handle start = t.locate(person);
//
//    }

//
//
//
//    // number of red and blue points
//    int m; std::cin >> m;
//    int n; std::cin >> n;
//
//    // read the red points (cancer cells)
//    for (int i=0; i<m; ++i) {
//        int x; std::cin >> x;
//        int y; std::cin >> y;
//        // set up <= constraint for point inside/on circle:
//        //   -alpha x - beta y - gamma <= -x^2 - y^2
//        lp.set_a (alpha, i, -x);
//        lp.set_a (beta,  i, -y);
//        lp.set_a (gamma, i, -1);
//        lp.set_b (       i, -x*x - y*y);
//    }
//    // read the blue points (healthy cells)
//    for (int j=0; j<n; ++j) {
//        int x; std::cin >> x;
//        int y; std::cin >> y;
//        // set up <= constraint for point outside circle:
//        //   alpha x + beta y + gamma + delta <= x^2 + y^2
//        lp.set_a (alpha, m+j, x);
//        lp.set_a (beta,  m+j, y);
//        lp.set_a (gamma, m+j, 1);
//        lp.set_a (delta, m+j, 1);
//        lp.set_b (       m+j, x*x + y*y);
//    }
//
//    // objective function: -delta (the solver minimizes)
//    lp.set_c(delta, -1);
//
//    // enforce a bounded problem:
//    lp.set_u (delta, true, 1);
//
//    // solve the program, using ET as the exact type
//    Solution s = CGAL::solve_linear_program(lp, ET());
//    assert (s.solves_linear_program(lp));
//
//    // output exposure center and radius, if they exist
//    if (s.is_optimal() && (s.objective_value() < 0)) {
//        // *opt := alpha, *(opt+1) := beta, *(opt+2) := gamma
//        CGAL::Quadratic_program_solution<ET>::Variable_value_iterator
//                opt = s.variable_values_begin();
//        CGAL::Quotient<ET> alpha = *opt;
//        CGAL::Quotient<ET> beta = *(opt+1);
//        CGAL::Quotient<ET> gamma = *(opt+2);
//        std::cout << "There is a valid exposure:\n";
//        std::cout << " Center = ("        // (alpha/2, beta/2)
//                  << alpha/2 << ", " <<  beta/2
//                  << ")\n";
//        std::cout << " Squared Radius = " // gamma + alpha^2/4 + beta^2/4
//                  << gamma + alpha*alpha/4 + beta*beta/4 << "\n";
//    } else
//        std::cout << "There is no valid exposure.";
//    std::cout << "\n";
}

int main() {
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}