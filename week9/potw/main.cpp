//
// Created by Hidde Lycklama on 11/12/18.
//

#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <vector>

typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Quotient<ET> SolT;

typedef double IT;

double ceil_to_double (const SolT &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a++;
    while (a - 1 >= x) a--;
    return a;
}


struct Point {
    int x;
    int y;

    Point(int xx, int yy): x(xx), y(yy) {};
    Point() = default;
};

bool cmp_x(const Point &p1, const Point &p2) {
    return p1.x < p2.x;
}
bool cmp_y(const Point &p1, const Point &p2) {
    return p1.y < p2.y;
}

void testcase() {
    int n, m, h, w;
    std::cin >> n >> m >> h >> w;

    Program lp(CGAL::SMALLER, true, 1, false, 0);

    double wDouble = (double)w;
    double hDouble = (double)h;

    // TODO: Change to include height
    const IT perimeter = 2.0 * wDouble + 2.0 * hDouble;
    const IT widthMultiplier = wDouble / 2;
    const IT heightMultiplier = hDouble / 2;

    std::vector<Point> points(n);

    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;

        Point p(x, y);
        points[i] = p;

        lp.set_c(i, -perimeter); // ci... etc.

//        lp.set_u(i, true, 1);
    }

    int constraints = 0;

    for (int a = 0; a < n; a++) {
        for (int b = a + 1; b < n; b++) {
            // Compare, see what we need to constrain (what may overlap)
            const int dx = abs(points[a].x - points[b].x);
            const int dy = abs(points[a].y - points[b].y);

//            std::cout << "dx " << dx << ", dy " << dy << " ";

            if (((double)dx / wDouble) > ((double)dy / hDouble)) {
                // constrain width
                lp.set_a(a, constraints, widthMultiplier);
                lp.set_a(b, constraints, widthMultiplier);
                lp.set_b(constraints, dx);
//                std::cout << "w ";
            } else {
                // constrain height
                lp.set_a(a, constraints, heightMultiplier);
                lp.set_a(b, constraints, heightMultiplier);
                lp.set_b(constraints, dy);
//                std::cout << "h ";
            }
            constraints++;
        }
    }

    const int entriesSoFar = constraints;

    std::vector<Point> old_x(m);

    for (int i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;

        old_x[i] = Point(x, y);

    } // ignore

    for (int i = 0; i < n; i++) {
        // find lower and higher x/y per point, constrain...
        auto point = &points[i];

        const int row = entriesSoFar + i;
        IT mm = IT(INT_MAX);
        bool width = false;

        for (int a = 0; a < m; a++) {
            auto old = &old_x[a];
//            std::cout << "Point " << a << std::endl;
            const IT dxl = abs(point->x - old->x) / wDouble;
            const IT dyl = abs(point->y - old->y) / hDouble;

            // if higher
//            const int xN = point.x - old.x > 0 ? 1 : -1;
//            const int yN = point.y - old.y > 0 ? 1 : -1;

            if (dxl > dyl) {
                mm = std::min(mm, dxl);
                width = true;
//                // constrain width
////                int up = dxl < 0 ? -1 * dxl : dxl;
//
////                std::cout << " Constraining extra " << i << " to not cause x to be " << (xN == 1 ? "less" : "more") << " than " << up << " - " << widthMultiplier << std::endl;
//                lp.set_a(i, row, widthMultiplier);
//                lp.set_b(row, dxl - widthMultiplier);
//
//
            } else {
                mm = std::min(mm, dyl);
                width = false;
////                int up = dyl < 0 ? -1 * dyl : dyl;
//                // constrain height
//                lp.set_a(i, row, heightMultiplier);
//                lp.set_b(row, dyl - heightMultiplier);
            }
        }
        if (width) {
            // w
            lp.set_a(i, row, widthMultiplier);
            lp.set_b(row, (mm * w) - widthMultiplier);
        } else {
            // h
            lp.set_a(i, row, heightMultiplier);
            lp.set_b(row, (mm * h) - heightMultiplier);
        }

    }
//    std::cerr << "Calc" << std::endl;
    CGAL::print_linear_program(std::cerr, lp);

    Solution s = CGAL::solve_linear_program(lp, ET());
//    std::cout << s <<std::endl;
    std::cout << std::setprecision(0) << std::fixed << ceil_to_double(-s.objective_value()) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}