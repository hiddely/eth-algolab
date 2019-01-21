//
// Created by Hidde Lycklama on 12/3/18.
//

#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
#include <stack>

typedef std::multiset<int> Window;
typedef std::deque<int> History;

struct Node {
    int v;
    Window window;
    History ref;

    Node(int a, Window b, History c): v(a), window(b), ref(c) {};

    Node() = default;
};


void testcase(int t) {
    int n, m, k;
    std::cin >> n >> m >> k;

    std::vector<int> temps(n);
    for (int i = 0; i < n; i ++) {
        std::cin >> temps[i];
    }

//    std::vector<int> predmap(n);
    std::vector<std::deque<int> > adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        adj_list[u].push_back(v);
//        adj_list[v].push_back(u);
//        leafs[u] = false;
    }

//    std::cerr << "Start main loop" << std::endl;
    std::set<int> starts;
    std::stack<Node> toVisit;
    std::vector<bool> visited(n);
//    std::multiset<int> window;
    toVisit.push(Node(0, Window(), History()));
    while (!toVisit.empty()) {
        auto cur = &toVisit.top();
        int c = cur->v;
        Window window = cur->window;
        History refs = cur->ref;
        toVisit.pop();

//        std::cerr << "Visiting " << c << " " << temps[c] << std::endl;

        window.insert(temps[c]);
        refs.push_back(c);

        int size = window.size();
        if (size > m) {
//            std::cerr << "Window too large " << std::endl;
            window.erase(window.find(temps[refs.front()]));
            refs.pop_front();
            size--;
        }

        if (size == m && *window.rbegin() - *window.begin() <= k) {
//                    std::cerr << max << " - " << min << " is alright. Pushing " << current << ", from: " << current << std::endl;
            starts.insert(refs.front());
        }

        std::deque<int> * nexts = &adj_list[c];
        for (auto iter = nexts->begin(); iter != nexts->end(); iter++) {
            toVisit.push(Node(*iter, window, refs));
        }
    }


    if (starts.empty()) {
        std::cout << "Abort mission" << std::endl;
    } else {
        for (auto iter = starts.cbegin(); iter != starts.cend(); iter++) {
            std::cout << *iter << " ";
        }
        if (t > 1) {
            std::cout << "\n";
        } else {
            std::cout << std::endl;
        }
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase(t);
    }
    return 0;
}