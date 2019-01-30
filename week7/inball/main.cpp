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

        const int R = d;

        Program lp(CGAL::SMALLER, false, 0, false, 0); // other vars may be nonnegative!
        lp.set_c(R, -1); // minimize
        lp.set_l(R, true, 0); //

        for (int i = 0; i < n; i++) {
            double norm = 0;
            for (int j = 0; j < d; j++) {
                int a; std::cin >> a;
                lp.set_a(j, i, a);
                norm += a * a;
            }
            int b; std::cin >> b;
            lp.set_b(i, b);

            lp.set_a(R, i, std::floor(std::sqrt(norm))); // set R
        }

        Solution sol = CGAL::solve_linear_program(lp, ET());

        if (sol.status() == CGAL::QP_INFEASIBLE) {
            std::cout << "none" << std::endl;
        } else if (sol.status() == CGAL::QP_UNBOUNDED) {
            std::cout << "inf" << std::endl;
        } else {
            std::cout << floor_to_double(-sol.objective_value()) << std::endl;
        }

    }
    return 0;
}