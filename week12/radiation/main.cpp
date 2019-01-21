//
// Created by Hidde Lycklama on 1/8/19.
//

#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <tuple>

// choose exact integral type
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::tuple<int, int, int> Cell;


bool findPoly(const int d, const int h, const int t, const std::vector<Cell> &healthy, const std::vector<Cell> &tumor) {
    Program lp (CGAL::SMALLER, false, 0, false, 0);
    const int OFFSET_TUMOR = h;

//    lp.set_c0(1);
    const int delta = 0;
    lp.set_c(delta, -1); // max delta
    lp.set_l(delta, true, 0);
    lp.set_u(delta, true, 1);

    for (int i = 0; i < h; i++) {
        lp.set_b(i, 0);
        lp.set_a(delta, i, 1);

        int index = 1;
        for (int vx = 0; vx <= d; vx++) {
            for (int vy = 0; vy <= d - vx; vy++) {
                for (int vz = 0; vz <= d - vx - vy; vz++) {
                    // do stuff
//                    std::cerr << std::get<0>(healthy[i]) << " " << std::get<1>(healthy[i]) << " " << std::get<2>(healthy[i]) << std::endl;
                    lp.set_a(index, i, std::pow(std::get<0>(healthy[i]), vx) * std::pow(std::get<1>(healthy[i]), vy) * std::pow(std::get<2>(healthy[i]), vz));
//                    lp.set_c(index, -1); // poly
                    index++;

//                    lp.set_a(index, i)
                }
            }
        }
    }

    for (int i = 0; i < t; i++) {

        lp.set_b(OFFSET_TUMOR + i, 0);
        lp.set_r(OFFSET_TUMOR + i, CGAL::LARGER);
        lp.set_a(delta, OFFSET_TUMOR + i, -1);

        int index = 1;
        for (int vx = 0; vx <= d; vx++) {
            for (int vy = 0; vy <= d - vx; vy++) {
                for (int vz = 0; vz <= d - vx - vy; vz++) {
// do stuff
                    lp.set_a(index, OFFSET_TUMOR + i, std::pow(std::get<0>(tumor[i]), vx) * std::pow(std::get<1>(tumor[i]), vy) * std::pow(std::get<2>(tumor[i]), vz));
                    index++;
                }
            }
        }
    }
//    std::cerr << "Looped " << d << std::endl;
//    CGAL::print_linear_program(std::cerr, lp);
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
//    std::cerr << s.objective_value() << " aa " << std::endl;
    return s.status() != CGAL::QP_INFEASIBLE && s.objective_value() != 0;
}

void testcase() {
    int h, t;
    std::cin >> h >> t;

    std::vector<Cell> healthy(h);
    std::vector<Cell> tumor(t);


    for (int i = 0; i < h; i++) {
        int x, y, z;
        std::cin >> x >> y >> z;
        healthy[i] = Cell(x, y, z);
    }
    for (int i = 0; i < t; i++) {
        int x, y, z;
        std::cin >> x >> y >> z;
        tumor[i] = Cell(x, y, z);
    }

    bool found = false;
    for (int i = 0; i <= 30; i++) {
        if (findPoly(i, h, t, healthy, tumor)) {
            found = true;
            std::cout << i << std::endl;
            break;
        }
    }
    if (!found) {
        std::cout << "Impossible!" << std::endl;
    }
//    std::cerr << s << std::endl;
}

int main() {
    int N; std::cin >> N;
    while (N-- > 0) {
        testcase();
    }
    return 0;
}