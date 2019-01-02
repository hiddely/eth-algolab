//
// Created by Hidde Lycklama on 10/4/18.
//

#include <iostream>
#include <iomanip>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include <CGAL/number_utils.h>
#include <cmath>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 Ray;
typedef K::Segment_2 S;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return a;
}

void testcase(int n) {
    long x; long y; long a; long b;
    std::cin >> x; std::cin >> y; std::cin >> a; std::cin >> b;

    P rayStart(x, y), rayEnd(a, b);
    Ray ray(rayStart, rayEnd);

    S intersegment;
    bool hasIntersect = false;

    for (int i = 0; i < n; i ++ ){
        long r; long s; long t; long u;
        std::cin >> r; std::cin >> s; std::cin >> t; std::cin >> u;

        P o1(r, s), o2(t, u);
        K::Segment_2 segment(o1, o2);

        if (hasIntersect) {

            if (CGAL::do_intersect(intersegment, segment)) {
                // check intersection
                auto o = CGAL::intersection(intersegment, segment);

                if (o) {
                    if (const P* op = boost::get<P>(&*o)) {
                        intersegment = S(ray.source(), *op);
                    }
                    else if (const S* os = boost::get<S>(&*o)) {
                        if (CGAL::has_smaller_distance_to_point(rayStart, os->target(), os->source())) {
                            // source is further away than target
                            intersegment = S(ray.source(), os->target());
                        } else {
                            // source is further away than target
                            intersegment = S(ray.source(), os->source());
                        }
                    }
                }
            }
        } else
        if (CGAL::do_intersect(ray, segment)) {
            // Initialize intersection variable
            auto o = CGAL::intersection(ray, segment);
            if (o) {
                if (const P* op = boost::get<P>(&*o)) {
                    intersegment = S(ray.source(), *op);
                    hasIntersect = true;
                } else if (const S* os = boost::get<S>(&*o)) {
                    if (CGAL::has_smaller_distance_to_point(rayStart, os->target(), os->source())) {
                        // source is further away than target
                        intersegment = S(ray.source(), os->target());
                    } else {
                        // target is further away than source
                        intersegment = S(ray.source(), os->source());
                    }
                    hasIntersect = true;
                }
            }
        }
    }
    if (hasIntersect) {

        std::cout << std::fixed << std::setprecision(0) << floor_to_double(intersegment.target().x()) << " " << floor_to_double(intersegment.target().y()) << std::endl;
//        std::cout << std::fixed << std::setprecision(0) << floor_to_double(intersegment.source().x()) << " " << floor_to_double(intersegment.source().y()) << std::endl;

    } else {
        std::cout << "no" << std::endl;
    }

//    std::cout << ray.source() << std::endl;
//    for (int i = 0; i < n; i ++ ){
//        long r; long s; long t; long u;
//        std::cin >> r; std::cin >> s; std::cin >> t; std::cin >> u;
//
//        P o1(r, s), o2(t, u);
//        K::Segment_2 segment(o1, o2);
//
//        if (hasIntersect) {
//            if (
////                    (CGAL::has_larger_distance_to_point(rayStart, intersection, o1) ||
////                CGAL::has_larger_distance_to_point(rayStart, intersection, o2)) &&
//                CGAL::do_intersect(ray, segment)) {
//                // check intersection
//                auto o = CGAL::intersection(ray, segment);
//
//                if (o) {
//                    if (const P* op = boost::get<P>(&*o)) {
//                        if (CGAL::has_smaller_distance_to_point(rayStart, *op, intersection)) {
//                            intersection = *op;
//                        }
//                    } else if (const S* os = boost::get<S>(&*o)) {
//                        if (CGAL::has_smaller_distance_to_point(rayStart, os->target(), os->source())) {
//                            // source is further away than target
//                            if (CGAL::has_smaller_distance_to_point(rayStart, os->target(), intersection)) {
//                                intersection = os->target();
//                            }
//                        } else {
//                            // source is further away than target
//                            if (CGAL::has_smaller_distance_to_point(rayStart, os->source(), intersection)) {
//                                intersection = os->source();
//                            }
//                        }
//                    }
//                }
//
//            }
//        } else
//            if (CGAL::do_intersect(ray, segment)) {
//            // Initialize intersection variable
//            auto o = CGAL::intersection(ray, segment);
//            if (o) {
//                if (const P* op = boost::get<P>(&*o)) {
//                    intersection = *op;
//                    hasIntersect = true;
//                } else if (const S* os = boost::get<S>(&*o)) {
//                    if (CGAL::has_smaller_distance_to_point(rayStart, os->target(), os->source())) {
//                        // source is further away than target
//                        intersection = os->target();
//                    } else {
//                        // target is further away than source
//                        intersection = os->source();
//                    }
//                    hasIntersect = true;
//                }
//            }
//        }
//    }
//    if (hasIntersect) {
//
//        std::cout << std::fixed << std::setprecision(0) << floor_to_double(intersection.x()) << " " << floor_to_double(intersection.y()) << std::endl;
//    } else {
//        std::cout << "no" << std::endl;
//    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::setprecision(0);
    while (true) {
        int n; std::cin >> n;
        if (n == 0) {
            break;
        }
        testcase(n);
    }
    return 0;
}