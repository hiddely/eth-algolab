//
// Created by Hidde Lycklama on 11/7/18.
//


#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Quotient<ET> SolT;

double floor_to_double(const SolT x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

int main() {
    while (true) {
        int n; std::cin >> n;
        if (n == 0) {
            break;
        }
        int d; std::cin >> d;

        Program lp(CGAL::SMALLER, false, 0, false, 0);

        for (int a = 0; a < n; a++) {
            int sum = 0;
            for (int b = 0; b < d; b++) {
                int v; std::cin >> v;
                lp.set_a(b, a, v);
                sum += v*v;
            }
            int mult = std::sqrt(sum);
            int v; std::cin >> v;
            lp.set_b(a, v);
            lp.set_a(d, a, mult);
        }

        for (int b = 0; b < d; b++) {
            lp.set_c(d, -1);
//            lp.set_d(b, b, -2);
//            lp.set_d(d + b, b, 1);
//            lp.set_d(d + b, d + b, -2);
        }

        lp.set_l(d, true, 0);

        Solution s = CGAL::solve_quadratic_program(lp, ET());
        assert (s.solves_quadratic_program(lp));

//        std::cout << s << std::endl;

        if (s.is_unbounded()) {
            std::cout << "inf" << std::endl;
        } else
        if (!s.is_optimal()) {
            std::cout << "none" << std::endl;
        } else {
            std::cout << std::setprecision(12) << (floor_to_double(-s.objective_value())) << std::endl;
        }
    }
    return 0;
}