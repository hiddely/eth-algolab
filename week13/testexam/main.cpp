// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Point_2  Point;

// choose exact integral type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET> x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase() {
    // by default, we have an LP with Ax <= b and no bounds for
    // the four variables alpha, beta, gamma, delta
    Program lp (CGAL::SMALLER, true, 0, false, 0);

    int n, m, c;
    std::cin >> n >> m >> c;

    std::vector<int> supplies(n);
    std::vector<int> percs(n);
    std::vector<Point> stadiums(n);
    std::vector<Point> warehouses(m);
    for (int i = 0; i < n; i++) {
        int x, y, s, a;
        std::cin >> x >> y >> s >> a;

        supplies[i] = s;
        percs[i] = a;
        stadiums[i] = Point(x, y);
    }

    int OFFSET_SUP = 0;
    int OFFSET_DEM = n;
    int OFFSET_ALC = n + m;

    for (int i = 0; i < m; i++) {
        int x, y, d, u;
        std::cin >> x >> y >> d >> u;

        lp.set_b(OFFSET_DEM + i, d); // Demand constraint
        lp.set_r(OFFSET_DEM + i, CGAL::EQUAL); // must equal

        lp.set_b(OFFSET_ALC + i, u * 100); // Alcohol constraint

        warehouses[i] = Point(x, y);
    }

    Triangulation t;
    t.insert(stadiums.begin(), stadiums.end());
    t.insert(warehouses.begin(), warehouses.end());

    std::vector<double> revenues(n * m);

    // n * a, m * b
    for (int a = 0; a < n; a++) {
        for (int b = 0; b < m; b++) {
            int r; std::cin >> r; // r_a_b
            // a = 0, b = 0.
            const int VAR_INDEX = a * m + b;

//            const int VAR_DEM = n * b + a;
//            const int VAR_SUP = m * a + b;

//            lp.set_c(VAR_SUP, -r);
            revenues[VAR_INDEX] = r;

            lp.set_a(VAR_INDEX, a, 1); // Supply constraint
            lp.set_a(VAR_INDEX, OFFSET_DEM + b, 1); // Demand constraint
            lp.set_a(VAR_INDEX, OFFSET_ALC + b, percs[a]); // Alcohol constraint
        }

        // Supply: Mij + M
        lp.set_b(a, supplies[a]); // spply constraint
    }

    for (int i = 0; i < c; i++) {
        int x, y, r;
        std::cin >> x >> y >> r;

        Point contCenter = Point(x, y);
        Point nearest = t.nearest_vertex(contCenter)->point();

        K::FT squared = K::FT(r * r);
        if (CGAL::squared_distance(contCenter, nearest) < squared) {
            // we are interested in the contour line
//            std::cerr << "Found interesting contour !!! " << contCenter << ", " << squared << std::endl;
            for (int a = 0; a < n; a++) {
                bool inStadium = CGAL::squared_distance(stadiums[a], contCenter) < squared;

                for (int b = 0; b < m; b++) {
                    const int VAR_INDEX = m * a + b;

                    bool inWarehouse = CGAL::squared_distance(warehouses[b], contCenter) < squared;
                    if (inStadium != inWarehouse) {
                        revenues[VAR_INDEX] -= 0.01;
//                        std::cerr << inStadium << " != " << inWarehouse << ", decreasing: " << revenues[VAR_INDEX] << " " << VAR_INDEX << std::endl;
                    }
                }
            }
        }
    }

    for (int a = 0; a < n; a++) {
        for (int b = 0; b < m; b++) {
            const int VAR_SUP = m * a + b;
//            std::cerr << VAR_SUP << ": " << revenues[VAR_SUP] << std::endl;
            lp.set_c(VAR_SUP, -revenues[VAR_SUP]);
        }
    }

//    CGAL::print_linear_program(std::cerr, lp);
//    std::cerr << CGAL::print_linear_program(lp,) << std::endl;

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    if (s.is_infeasible()) {
//        std::cerr << s << std::endl;
        std::cout << "RIOT!" << std::endl;
    } else {
//        std::cout << 0 << std::endl;
        std::cout << std::fixed << std::setprecision(0) << floor_to_double(-s.objective_value()) << std::endl;
    }

}


int main() {
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}