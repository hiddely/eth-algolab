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

void DFS(int v, Window &window, int s, int m, int k, History &ref, std::vector<std::deque<int> > &adj_list, std::vector<int> &temps, std::set<int> &res) {

//    std::cerr << "Attempting " << v << " " << " " << std::endl;
    ref.push_back(v);
    int val = 0;
    if (s >= m) {
        window.erase(window.find(temps[*ref.begin()]));
        val = ref.front();
//        std::cerr << v << " Saving " << val << std::endl;
        ref.pop_front();
    }
    window.insert(temps[v]);

//    std::cerr << "Success " << v << " " << val << std::endl;
//    for (auto iter = window.begin(); iter != window.end(); iter++) {
//        std::cerr << *iter << " ";
//    }
//    std::cerr << std::endl;

    if (window.size() == m && *window.rbegin() - *window.begin() <= k) {
        res.insert(ref.front());
    }

    for (auto iter = adj_list[v].begin(); iter != adj_list[v].end(); iter++) {
        DFS(*iter, window, s + 1, m, k, ref, adj_list, temps, res);
    }


    if (s >= m) {
//        std::cerr << v <<  " Putting back " << val << " " << temps[val] << std::endl;
        ref.push_front(val);
        window.insert(temps[val]);
    }
//    std::cerr << "Erasing " << v << " " << temps[v] << std::endl;
    window.erase(window.find(temps[v]));
    ref.pop_back();
}


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
    Window window;
    History history;
//    window.erase(window.find(*history.begin()));
    DFS(0, window, 0, m, k, history, adj_list, temps, starts);

//    std::cerr << window.empty() << std::endl;

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