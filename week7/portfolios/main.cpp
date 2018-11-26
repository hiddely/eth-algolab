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

        const int R = 0;
        const int C = 1;
        const int V = 2;

        for (int i = 0; i < n; i++) {
            int c, r; std::cin >> c >> r;
            lp.set_a(i, C, c);
            lp.set_a(i, R, r);
        }

        for (int a = 0; a < n; a++) {
            for (int b = 0; b < n; b++) {
                int v; std::cin >> v;
                if (a < b) {
                    continue;
                }
                lp.set_d(a, b, v * 2.0);
            }
        }

        for (int i = 0; i < m; i++) {
            int c, r, v;
            std::cin >> c >> r >> v;
            lp.set_b(R, r);
            lp.set_r(R, CGAL::LARGER);
            lp.set_b(C, c);

            assert(lp.is_nonnegative());
            Solution s = CGAL::solve_nonnegative_quadratic_program(lp, ET());
            assert (s.solves_quadratic_program(lp));

            if (!s.is_optimal()) {
                std::cout << "No." << std::endl;
            } else if (s.objective_value() <= v) {
                std::cout << "Yes." << std::endl;
            } else {
                std::cout << "No." << std::endl;
            }
        }


    }
    return 0;
}