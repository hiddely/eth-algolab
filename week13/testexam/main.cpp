// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
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
    Program lp (CGAL::SMALLER, false, 0, false, 0);

    int n, m, c;
    std::cin >> n >> m >> c;

    std::vector<int> supplies(n);
    std::vector<int> percs(n);
    for (int i = 0; i < n; i++) {
        int x, y, s, a;
        std::cin >> x >> y >> s >> a;

        supplies[i] = s;
        percs[i] = a;
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
    }

    // n * a, m * b
    for (int a = 0; a < n; a++) {
        for (int b = 0; b < m; b++) {
            int r; std::cin >> r;

            const int VAR_DEM = m * b + a;
            const int VAR_SUP = n * a + b;

            lp.set_c(VAR_SUP, -r);

            lp.set_a(VAR_SUP, a, 1); // Supply constraint
            lp.set_a(VAR_DEM, a + OFFSET_DEM, 1); // Demand constraint
            lp.set_a(VAR_DEM, a + OFFSET_ALC, percs[a]); // Alcohol constraint
        }

        // Supply: Mij + M
        lp.set_b(a, supplies[a]); // spply constraint
    }

    for (int i = 0; i < c; i++) {
        int x, y, r;
        std::cin >> x >> y >> r;

    }

//    CGAL::print_linear_program(std::cerr, lp);
//    std::cerr << CGAL::print_linear_program(lp,) << std::endl;

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    if (s.is_infeasible()) {
        std::cout << "RIOT!" << std::endl;
    } else {
//        std::cout << 0 << std::endl;
        std::cout << floor_to_double(-s.objective_value()) << std::endl;
    }


//    const int alpha = 0;
//    const int beta  = 1;
//    const int gamma = 2;
//    const int delta = 3;
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

}


int main() {
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}