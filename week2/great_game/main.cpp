//
// Created by Hidde Lycklama on 10/2/18.
//

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <limits>

struct Vertex {
    int pos;
    std::vector<Vertex*> edges;

    bool operator < (const Vertex& str) const
    {
        return (pos < str.pos);
    }

    Vertex(int p): pos(p) {}
    Vertex() = default;
};

struct Option {
    int good;
    int goodPos;
    int bad;
    int badPos;

    Option(int g, int b, int gp, int bp): good(g), goodPos(gp), bad(b), badPos(bp) {}
    Option() = default;
};


//Option findPathBFS(int start, int target) {
//    // Does r or b have a shorter path?
//    std::queue<Vertex> nodes;
//    nodes.push(positions[start - 1]);
//    std::map<int, int> parentNodes;
//    std::vector<bool> visited(target);
//    visited[start - 1] = true;
//    bool foundShortest = false;
//    // We dont need a visited array as it always goes up
//    // find path for r
//
//    while (!nodes.empty()) {
//        Vertex node = nodes.front();
//        nodes.pop();
////        std::cout << "Visiting " << node.pos;
//        visited[node.pos - 1] = true;
//
//        std::vector<Vertex*>::iterator iterator = node.edges.begin();
//        for (iterator = node.edges.begin(); iterator != node.edges.end(); iterator++) {
//            Vertex v = (**iterator);
//            if (visited[v.pos - 1]) {
//                continue;
//            }
//            parentNodes[v.pos] = node.pos;
//            nodes.push(v);
//
//            if (v.pos == target) {
//                // We have the target
//                found = true;
//                break;
//            }
//        }
//    }
//
//    if (found) {
//        int node = target;
//        std::vector<int> out;
//        while (node != start) {
//            out.push_back(node);
////            std::cout << "N " << node << " to " << parentNodes[node] << std::endl;
//            node = parentNodes[node];
//        }
//        std::sort(out.begin(), out.end());
//        Vertex min = **std::min_element(positions[start - 1].edges.begin(), positions[start - 1].edges.end());
//        return Option(out[0], min.pos); // TODO: Fix second term;
//    }
////    while (!nodes.empty()) {
////        Vertex node = nodes.front();
////        nodes.pop();
//////        std::cout << "Visiting " << node.pos;
////        visited[node.pos - 1] = true;
////
////        std::vector<Vertex*>::iterator iterator = node.edges.begin();
////        for (iterator = node.edges.begin(); iterator != node.edges.end(); iterator++) {
////            Vertex v = (**iterator);
////            if (visited[v.pos - 1]) {
////                continue;
////            }
////            parentNodes[v.pos] = node.pos;
////            nodes.push(v);
////
////            if (v.pos == target) {
////                // We have the target
////                found = true;
////                break;
////            }
////        }
////    }
////
////    if (found) {
////        int node = target;
////        std::vector<int> out;
////        while (node != start) {
////            out.push_back(node);
//////            std::cout << "N " << node << " to " << parentNodes[node] << std::endl;
////            node = parentNodes[node];
////        }
////        std::sort(out.begin(), out.end());
////        Vertex min = **std::min_element(positions[start - 1].edges.begin(), positions[start - 1].edges.end());
////        return Option(out[0], min.pos); // TODO: Fix second term;
////    }
//    return {-1, -1};
//}

void testcase() {
    int n; std::cin >> n; // positions
    int m; std::cin >> m; // transitions
    int r; std::cin >> r; // red meeple start
    int b; std::cin >> b; // black meeple start
    r--; b--;

//    std::cout << n << " " << m << std::endl;
//    std::cout << r << " " << b << std::endl;

    std::vector<Vertex> positions(n);
    for (int i = 0; i < n; i++) {
        positions[i] = Vertex(i);
    }
    for (int i = 0; i < m; i++) {
        int u; std::cin >> u;
        int v; std::cin >> v;
        positions[u - 1].edges.push_back(&positions[v - 1]);
    }

//    for (int i = 0; i < m; i++) {
//        for (int a = 0; a < m; a++) {
//            if (positions[i].edges[a] != nullptr) {
//                std::cout << positions[i].edges[a]->pos << ", ";
//            }
//        }
//        std::cout << std::endl;
//    }

    std::vector<Option> options(n);
    options[n - 1] = Option(0, 0, 0, 0);
    for (int i = n - 2; i >= 0; i--) {

        // Find the ordering
        Vertex node = positions[i];
        int good = std::numeric_limits<int>::max();
        int goodPos = -1;
        int bad = std::numeric_limits<int>::min();
        int badPos = -1;

        std::vector<Vertex*>::iterator iterator = node.edges.begin();
        for (iterator = node.edges.begin(); iterator != node.edges.end(); iterator++) {
            Vertex v = (**iterator); // v is always higher
            Option vOption = options[v.pos];
            if (good > vOption.bad + 1) {
                good = vOption.bad + 1;
                goodPos = v.pos;
            }
            if (bad < vOption.good + 1) {
                bad = vOption.good + 1;
                badPos = v.pos;
            }
        }

//        std::cout << "Option " << i << ": " << good << " " << bad << std::endl;

        options[i] = Option(good, bad, goodPos, badPos);

//        std::cout << "Opt " << i + 1 << ": " << options[i].good << std::endl;
    }

//    for (int i = 0; i < n; i++) {
//
//        std::cout << i << " " << "Good: " << options[i].good << std::endl;
//        std::cout << i << " " << "Bad: " << options[i].bad << std::endl;
//    }

    for (int i = 0; i < 2*n; i++) {
        // determine turn
        int redTurn = ((i + 1) / 2) % 2 == 0;
        int optimalTurn = (i / 2) % 2 == 0;
        if (redTurn && optimalTurn) {
//            std::cout << options[r].goodPos << " -> ";
//            std::cout << "Moving r to " << options[r].goodPos + 1 << " (good)" << std::endl;
            r = options[r].goodPos;
        } else if (redTurn && !optimalTurn) {
//            std::cout << options[r].badPos << " -> ";
//            std::cout << "Moving r to " << options[r].badPos + 1 << " (bad)" << std::endl;
            r = options[r].badPos;
        } else if (!redTurn && optimalTurn) {
//            std::cout << "Moving b to " << options[b].goodPos + 1 << " (good)" << std::endl;
            b = options[b].goodPos;
        } else if (!redTurn && !optimalTurn) {
//            std::cout << "Moving b to " << options[b].badPos + 1 << " (bad)" << std::endl;
            b = options[b].badPos;
        }

        if (r == n - 1) {
            std::cout << 0 << std::endl;
            break;
        } else if (b == n - 1) {
            std::cout << 1 << std::endl;
            break;
        }
    }

//
//    int red = findPathLengthBFS(r, n);
//    int black = findPathLengthBFS(b, n);
//
//    std::cout << "red " << red << std::endl;
//    std::cout << "black " << black << std::endl;
//
//    std::cout << (red < black) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
