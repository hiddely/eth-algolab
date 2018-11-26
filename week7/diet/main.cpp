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
        int n; int m;
        std::cin >> n >> m;
        if (n == 0 && m == 0) {
            break;
        }
        Program lp (CGAL::SMALLER, true, 0, false, 0);

        for (int i = 0; i < n; i++) {
            int min; int max;
            std::cin >> min >> max;
            lp.set_b(i * 2, max); // <= max
            lp.set_b(i * 2 + 1, min);
            lp.set_r(i * 2 + 1, CGAL::LARGER);
        }

        for (int i = 0; i < m; i++) {
            int price;
            std::cin >> price;
            for (int a = 0; a < n; a++) {
                int contains; std::cin >> contains;
                lp.set_a(i, 2 * a, contains);
                lp.set_a(i, 2 * a + 1, contains);
            }
            lp.set_c(i, price);
        }

        assert(lp.is_nonnegative());
        Solution s = CGAL::solve_nonnegative_quadratic_program(lp, ET());
        assert (s.solves_quadratic_program(lp));

        if (!s.is_optimal()) {
            std::cout << "No such diet." << std::endl;
        } else {
            std::cout << std::setprecision(12) << floor_to_double(s.objective_value()) << std::endl;
        }
    }
    return 0;
}