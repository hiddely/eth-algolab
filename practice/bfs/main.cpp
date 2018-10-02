//
// Created by Hidde Lycklama on 9/24/18.
//

#include <iostream>
#include <vector>
#include <set>

struct Vertex {
    int label;
    std::set<Vertex*> neighbours;

    Vertex(int l, std::set<Vertex*> n) : label(l), neighbours(n) {}
};

void testcase() {
    int n; std::cin >> n;
    int m; std::cin >> m;
    int start; std::cin >> start;
    std::vector<Vertex> vertices(n);
    for (int i = 0; i < n; i++) {
        vertices[i] = Vertex(i)
    }

    for (int i = 0; i < m; i++) {
        int a; std::cin >> a;
        int b; std::cin >> b;
        vertices[a].neighbours.insert(b)
    }

    while
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
