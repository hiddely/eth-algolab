//
// Created by Hidde Lycklama on 10/27/18.
//

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct Bomb {
    int index;
    int t;
    int solvable;

    Bomb(int i, int time, int solv): index(i), t(time), solvable(solv) {};
    Bomb() = default;

    int parentIndex() const {
        int i = this->index;
        if (i == 0) {
            return -1;
        }
        if (i % 2 == 0) {
            return (i - 2) / 2;
        } else {
            return (i - 1) / 2;
        }
    }

};

struct Cmp {
    bool operator() (const Bomb &b1, const Bomb &b2) const {
        return b1.t >= b2.t;
    }
};

void testcase(int abb) {
    int n; std::cin >> n;
    std::vector<Bomb> bombs(n);
    for (int i = 0; i < n; i++) {
        int t; std::cin >> t;
        Bomb b = Bomb(i, t, 2);
//        std::cout << b.t << " " << b.parentIndex() << std::endl;
        if (i != 0) {
            Bomb parent = bombs[b.parentIndex()];
            b.t = std::min(b.t, parent.t - 1);
        }
        bombs[i] = b;
    }

    std::priority_queue<Bomb, std::vector<Bomb>, Cmp > toSolve;

    for (int i = (n - 1) / 2; i < n; i++) {
        toSolve.push(bombs[i]);
    }

    int time = 0;
    bool exploded = false;
    while (!toSolve.empty()) {
        auto next = toSolve.top();
        toSolve.pop();

//        std::cout << time << ": Solving " << next.t << std::endl;

        if (next.t <= time) {
            exploded = true;
            break;
        }

        int parentIndex = next.parentIndex();
        if (parentIndex >= 0) {
            // we havent finished yet
            auto nextBomb = bombs[parentIndex];
            nextBomb.solvable--;
            bombs[parentIndex] = nextBomb;
            if (nextBomb.solvable <= 0) {
                toSolve.push(nextBomb);
            } else {
//                std::cout << "Parent is not solvable yet " << parentIndex << std::endl;
            }
        } else {
//            std::cout << "Parent is nil" << std::endl;
        }

        time++;
    }

    if (exploded) {
        std::cout << "no" << std::endl;
    } else {
        std::cout << "yes" << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t > 0) {
        testcase(t);
        t--;
    }
}