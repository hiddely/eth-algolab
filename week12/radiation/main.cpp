//
// Created by Hidde Lycklama on 1/8/19.
//

#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
    Program lp (CGAL::SMALLER, false, 0, false, 0);

}

int main() {
    int N; std::cin >> N;
    while (N-- > 0) {
        testcase();
    }
    return 0;
}