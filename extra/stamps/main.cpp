// example: find the risk-optimal investment strategy
// in Swatch and Credit Suisse shares (data as in lecture)
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
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
    int l, s, w; std::cin >> l >> s >> w;

    std::vector<P> lights(l);
    for (int i = 0; i < l; i++) {
        int x, y; std::cin >> x >> y;
        Point p(x, y);
        lights[i] = p;
    }

    std::vector<P> stamps(s);
    std::vector<int> limits(s);
    for (int i = 0; i < l; i++) {
        int x, y, M; std::cin >> x >> y >> M;
        limits[i] = M;
        Point p(x, y);
        lights[i] = p;
    }



    // by default, we have a nonnegative QP with Ax >= b
    Program qp (CGAL::LARGER, true, 0, false, 0);

    // now set the non-default entries:
    const int sw = 0;
    const int cs = 1;

    // constraint on expected return: 0.1 sw + 0.51 cs >= rho
    qp.set_a(sw, 0, ET(1)/10);
    qp.set_a(cs, 0, ET(51)/100);
    qp.set_b(    0, rho);

    // strategy constraint: sw + cs = 1
    qp.set_a(sw, 1, 1);
    qp.set_a(cs, 1, 1);
    qp.set_b(    1, 1);
    qp.set_r(    1, CGAL::EQUAL); // override default >=

    // objective function: 0.09 sw^2 - 0.1 sw cs - 0.1 cs sw + 0.25 cs^2
    // we need to specify the entries of 2D, on and below the diagonal
    qp.set_d(sw, sw, ET(18)/100); //  0.09 sw^2
    qp.set_d(cs, sw, ET(-1)/10); // -0.05 cs sw
    qp.set_d(cs, cs, ET(1)/2);  //  0.25 cs^2

    // solve the program, using ET as the exact type
    assert(qp.is_nonnegative());
    Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
    assert(s.solves_quadratic_program(qp));

    // output
    if (s.status() == CGAL::QP_INFEASIBLE) {
        std::cout << "Expected return rate " << rho << " cannot be achieved.\n";
    } else {
        assert (s.status() == CGAL::QP_OPTIMAL);
        Solution::Variable_value_iterator opt = s.variable_values_begin();
        ET sw_ratio = opt->numerator() / opt->denominator();
        ET cs_ratio = (opt+1)->numerator() / (opt+1)->denominator();
        ET risk = s.objective_value().numerator() / s.objective_value().denominator();
        double sw_percent = ceil_to_double(100 * *opt);
        std::cout << "Minimum risk investment strategy:\n"
                  << sw_ratio << " ~ " << sw_percent << "%" << " Swatch\n"
                  << cs_ratio << " ~ " << 100-sw_percent << "%" << " CS\n"
                  << "Risk = " << risk
                  << " ~ ." << ceil_to_double(100 * s.objective_value())
                  << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}