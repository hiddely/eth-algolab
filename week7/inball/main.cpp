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
        int n; std::cin >> n;

        const int R = d;

        Program

        for (int i = 0; i < n; i++) {

        }

    }
    return 0;
}