// example: find the risk-optimal investment strategy
// in Swatch and Credit Suisse shares (data as in lecture)
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpz.h>
typedef int ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// round up to next integer double
double ceil_to_double(const SolT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}



void testcase() {
    // read minimum expected return
    int b, s, p;
    std::cin >> b >> s >> p;

    const int OFFSET_SAIL = b;

    // by default, we have a nonnegative QP with Ax <= b
    Program lp(CGAL::SMALLER, true, 0, true, 1);

    for (int i = 0; i < p; i ++) {
        int boat, sailor, val;
        std::cin >> boat >> sailor >> val;

        lp.set_c(i, -val); // inverse
        lp.set_a(i, boat, 1);
        lp.set_a(i, OFFSET_SAIL + sailor, 1);
    }

    for (int i = 0; i < b; i++) {
        lp.set_b(i, 1);
    }
    for (int i = 0; i < s; i++) {
        lp.set_b(OFFSET_SAIL + i, 1);
    }

//    assert(lp.is_nonnegative());
    Solution sol = CGAL::solve_nonnegative_linear_program(lp, ET());
//    assert(sol.solves_linear_program(lp));

    std::cout << CGAL::to_double(-sol.objective_value()) << std::endl;

    // output
//    if (s.status() == CGAL::QP_INFEASIBLE) {
//        std::cout << "Expected return rate " << rho << " cannot be achieved.\n";
//    } else {
//        assert (s.status() == CGAL::QP_OPTIMAL);
//        Solution::Variable_value_iterator opt = s.variable_values_begin();
//        ET sw_ratio = opt->numerator() / opt->denominator();
//        ET cs_ratio = (opt + 1)->numerator() / (opt + 1)->denominator();
//        ET risk = s.objective_value().numerator() / s.objective_value().denominator();
//        double sw_percent = ceil_to_double(100 * *opt);
//        std::cout << "Minimum risk investment strategy:\n"
//                  << sw_ratio << " ~ " << sw_percent << "%" << " Swatch\n"
//                  << cs_ratio << " ~ " << 100 - sw_percent << "%" << " CS\n"
//                  << "Risk = " << risk
//                  << " ~ ." << ceil_to_double(100 * s.objective_value())
//                  << "\n";
//    }

}
int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t-- > 0) {
        testcase();
    }
    return 0;
}
