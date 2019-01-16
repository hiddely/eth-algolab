//
// Created by Hidde Lycklama on 1/10/19.
//

#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


// round up to next integer double
double floor_to_double(const SolT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase() {
    int numZones, numJobs;
    std::cin >> numZones >> numJobs;

    Program lp (CGAL::SMALLER, true, 0, true, 1);

    const int OFFSET_JOB = numZones;

    for (int i = 0; i < numZones; i++) {
        int c; std::cin >> c;
        lp.set_c(i, c);
    }

    for (int i = 0; i < numJobs; i++) {
        int c; std::cin >> c;
        lp.set_c(OFFSET_JOB + i, -c);
    }

    std::vector<int> zCounts(numZones);

    for (int l = 0; l < numJobs; l++) {
        int n; std::cin >> n;
        for (int i = 0; i < n; i++) {
            int z; std::cin >> z;
            lp.set_a(OFFSET_JOB + l, z, 1);
            zCounts[z]++;
        }
    }
    for (int i = 0; i <numZones; i++) {
        if (zCounts[i] > 0) {
            lp.set_a(i, i, -zCounts[i]);
            lp.set_b(i, 0);
        }
    }

//    CGAL::print_linear_program(std::cerr, lp);

    Solution s = CGAL::solve_linear_program(lp, ET());
//    assert(s.solves_linear_program(lp));

    std::cout << floor_to_double(-s.objective_value()) << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}