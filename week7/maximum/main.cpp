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

double ceil_to_double (const SolT &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a++;
    while (a - 1 >= x) a--;
    return a;
}

int main() {

    while (true) {
        int p; std::cin >> p;
        if (p == 0) {
            break;
        }
        int a, b;
        std::cin >> a >> b;

        if (p == 1) {
            // by default, we have a nonnegative LP with Ax <= b
            Program lp (CGAL::SMALLER, true, 0, false, 0);

            const int X = 0;
            const int Y = 1;

            lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4); // x + y <= 4
            lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b); // 4x + 2y <= ab
            lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); // -x + y <= 1
            lp.set_c(Y, -b);
            lp.set_d(X, X, 2  * a);// lp.set_d(Y, Y, 0);

            assert(lp.is_nonnegative());
            Solution s = CGAL::solve_nonnegative_quadratic_program(lp, ET());
            assert (s.solves_quadratic_program(lp));

            if (!s.is_optimal()) {
                std::cout << "no" << std::endl;
            } else if (s.is_unbounded()) {
                std::cout << "unbounded" << std::endl;
            } else {
                std::cout << std::setprecision(12) << -ceil_to_double(s.objective_value()) << std::endl;
            }
        } else if (p == 2) {
//            // by default, we have a negative LP with Ax >= b
            Program lp (CGAL::LARGER, false, 0, false, 0);

            const int X = 0;
            const int Y = 1;
            const int Z = 2;

            lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4); // x + y >= 4
            lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a * b); // 4x + 2y + z >= -ab
            lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1); // -x + y >= -1
            lp.set_d(X, X, 2 * a); lp.set_c(Y, b); lp.set_d(Z, Z, 2);

            lp.set_u(X, true, 0);
            lp.set_u(Y, true, 0);

            Solution s = CGAL::solve_quadratic_program(lp, ET());
            assert (s.solves_quadratic_program(lp));

            if (!s.is_optimal()) {
                std::cout << "no" << std::endl;
            } else if (s.is_unbounded()) {
                std::cout << "unbounded" << std::endl;
            } else {
                std::cout << std::setprecision(12) << ceil_to_double(s.objective_value()) + 0.0 << std::endl;
            }

//            std::cout << s << std::endl;
        }
    }
    // by default, we have a nonnegative LP with Ax <= b
//    Program lp (CGAL::SMALLER, true, 0, false, 0);
//
//    // now set the non-default entries
//    const int X = 0;
//    const int Y = 1;
//    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 7);  //  x + y  <= 7
//    lp.set_a(X, 1, -1); lp.set_a(Y, 1, 2); lp.set_b(1, 4);  // -x + 2y <= 4
//    lp.set_u(Y, true, 4);                                   //       y <= 4
//    lp.set_c(Y, -32);                                       // -32y
//    lp.set_c0(64);                                          // +64
//
//    // solve the program, using ET as the exact type
//    Solution s = CGAL::solve_linear_program(lp, ET());
//    assert (s.solves_linear_program(lp));
//
//    // output solution
//    std::cout << s;
    return 0;
}
