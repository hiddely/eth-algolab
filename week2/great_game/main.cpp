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

void testcase() {
    int n; std::cin >> n; // positions
    int m; std::cin >> m; // transitions
    int r; std::cin >> r; // red meeple start
    int b; std::cin >> b; // black meeple start
    r--; b--;

    std::vector<Vertex> positions(n);
    for (int i = 0; i < n; i++) {
        positions[i] = Vertex(i);
    }
    for (int i = 0; i < m; i++) {
        int u; std::cin >> u;
        int v; std::cin >> v;
        positions[u - 1].edges.push_back(&positions[v - 1]);
    }

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

        options[i] = Option(good, bad, goodPos, badPos);

    }

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

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}
