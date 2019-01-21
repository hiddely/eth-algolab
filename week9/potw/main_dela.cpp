//
// Created by Hidde Lycklama on 11/12/18.
//

#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Point_2 Point;

typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Quotient<ET> SolT;

double ceil_to_double (const SolT &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a++;
    while (a - 1 >= x) a--;
    return a;
}

void testcase() {
    int n, m, h, w;
    std::cin >> n >> m >> h >> w;

    Program lp(CGAL::SMALLER, true, 1, false, 0);

    // TODO: Change to include height
    const ET perimeter = ET(2) * ET(w) + ET(2) * ET(h);
    const ET widthMultiplier = ET(w) / ET(2);
    const ET heightMultiplier = ET(h) / ET(2);

    std::vector<Point> points(n);

    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;

        Point p(x, y);
        points[i] = p;

        lp.set_c(i, -perimeter); // ci... etc.

//        lp.set_u(i, true, 1);
    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    for (int a = 0; a < n; a++) {
        for (int b = 0; b < n; b++) {
            if (a == b) {
                continue;
            }
            // Compare, see what we need to constrain (what may overlap)
            int dx = abs(points[a].x - points[b].x);
            int dy = abs(points[a].y - points[b].y);

//            std::cout << "dx " << dx << ", dy " << dy << " ";

            if ((ET(dx) / ET(w)) > (ET(dy) / ET(h))) {
                // constrain width
                lp.set_a(a, (a * n) + b, widthMultiplier);
                lp.set_a(b, (a * n) + b, widthMultiplier);
                lp.set_b((a * n) + b, dx);
//                std::cout << "w ";
            } else {
                // constrain height
                lp.set_a(a, (a * n) + b, heightMultiplier);
                lp.set_a(b, (a * n) + b, heightMultiplier);
                lp.set_b((a * n) + b, dy);
//                std::cout << "h ";
            }
        }
    }

    int entriesSoFar = n*n;

    std::vector<Point> old_x(m);

    for (int i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;

        old_x[i] = Point(x, y);

    } // ignore

    // for each point calculate where we can not go
    std::sort(old_x.begin(), old_x.end(), cmp_x);
    for (int a = 0; a < n; a++) {
        auto point = points[a];
        int minX, maxX, minY, maxY;
        // minX

    }

    std::sort(old_x.begin(), old_x.end(), cmp_x);
    for (int a = 0; a < n; a++) {
        // sorted by x ?
        auto point = points[a];

        auto upper = std::lower_bound(old_x.cbegin(), old_x.cend(), point, cmp_x);
        if (upper != old_x.cbegin()) {
            auto lower = upper - 1;
//            std::cout << "Found lower bound " << lower->x << " for " << point.x << " " << a << std::endl;
            const int dxl = abs(point.x - lower->x);
//            std::cout << "Setting " << widthMultiplier << " " << dxl << std::endl;
            const int dyl = abs(point.y - lower->y);
            if ((ET(dxl) / ET(w)) > (ET(dyl) / ET(h))) {
                lp.set_a(a, entriesSoFar, widthMultiplier);
                lp.set_b(entriesSoFar, dxl - widthMultiplier);
            }
//            } else {
//                lp.set_a(a, entriesSoFar, heightMultiplier);
//                lp.set_b(entriesSoFar, dyl - heightMultiplier);
//            }
            entriesSoFar++;
        }

        if (upper == old_x.cend()) {
            continue;
        }
//        std::cout << "Found upper bound " << upper->x << " for " << point.x << " " << a << std::endl;
        const int dxu = abs(upper->x - point.x);
//        std::cout << "Setting " << widthMultiplier << " " << dxu << std::endl;
        const int dyu = abs(point.y - upper->y);
        if ((ET(dxu) / ET(w)) > (ET(dyu) / ET(h))) {
            lp.set_a(a, entriesSoFar, widthMultiplier);
            lp.set_b(entriesSoFar, dxu - widthMultiplier);
        }
//        } else {
//            lp.set_a(a, entriesSoFar, heightMultiplier);
//            lp.set_b(entriesSoFar, dyu - heightMultiplier);
//        }
        entriesSoFar++;
    }

    std::sort(old_x.begin(), old_x.end(), cmp_y);
    for (int a = 0; a < n; a++) {
        // sorted by x ?
        auto point = points[a];

        auto upper = std::lower_bound(old_x.cbegin(), old_x.cend(), point, cmp_y);
        if (upper != old_x.cbegin()) {
            auto lower = upper - 1;
//            std::cout << "Found y" << std::endl;
//            std::cout << "Found lower bound " << lower->x << " for " << point.x << " " << a << std::endl;
            const int dxl = abs(point.x - lower->x);
            const int dyl = point.y - upper->y;
            if ((ET(dxl) / ET(w)) < (ET(dyl) / ET(h))) {
//                lp.set_a(a, entriesSoFar, widthMultiplier);
//                lp.set_b(entriesSoFar, dxl - widthMultiplier);
//            } else {
                lp.set_a(a, entriesSoFar, heightMultiplier);
                lp.set_b(entriesSoFar, dyl - heightMultiplier);
            }
            entriesSoFar++;
        }

        if (upper == old_x.cend()) {
            continue;
        }
//        std::cout << "Found y L" << std::endl;

//        std::cout << "Found upper bound " << upper->x << " for " << point.x << " " << a << std::endl;
        const int dxu = abs(point.x - upper->x);
        const int dyu = upper->y - point.y;
        if ((ET(dxu) / ET(w)) < (ET(dyu) / ET(h))) {
//            lp.set_a(a, entriesSoFar, widthMultiplier);
//            lp.set_b(entriesSoFar, dxu - widthMultiplier);
//        } else {
            lp.set_a(a, entriesSoFar, heightMultiplier);
            lp.set_b(entriesSoFar, dyu - heightMultiplier);
        }
        entriesSoFar++;
    }

//    for (int i = 0; i < n; i++) {
//        // find lower and higher x/y per point, constrain...
//        auto point = points[i];
//
//        for (int a = 0; a < m; a++) {
//            auto old = old_x[a];
////            std::cout << "Point " << a << std::endl;
//            const int dxl = abs(point.x - old.x);
//            const int dyl = abs(point.y - old.y);
//
//            const int row = entriesSoFar + (i * m) + a;
//
//            // if higher
////            const int xN = point.x - old.x > 0 ? 1 : -1;
////            const int yN = point.y - old.y > 0 ? 1 : -1;
//
//            if ((ET(dxl) / ET(w)) > (ET(dyl) / ET(h))) {
//                // constrain width
////                int up = dxl < 0 ? -1 * dxl : dxl;
//
////                std::cout << " Constraining extra " << i << " to not cause x to be " << (xN == 1 ? "less" : "more") << " than " << up << " - " << widthMultiplier << std::endl;
//                lp.set_a(i, row, widthMultiplier);
//                lp.set_b(row, dxl - widthMultiplier);
//            } else {
////                int up = dyl < 0 ? -1 * dyl : dyl;
//                // constrain height
//                lp.set_a(i, row, heightMultiplier);
//                lp.set_b(row, dyl - heightMultiplier);
//            }
//        }
//
//    }

    Solution s = CGAL::solve_linear_program(lp, ET());
//    std::cout << s <<std::endl;
    std::cout << std::setprecision(0) << std::fixed << ceil_to_double(-s.objective_value()) << std::endl;
}

int main() {
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}