#include <iostream>
#include <vector>
#include <climits>
#include <queue>

#define LAST_STANDING ((n - 3) / 2)
#define FIRST_GROUND ((n - 1) / 2)

struct Bomb {
    int index;
    int time;
    int solved_sub_bombs = 0;

    Bomb(int i, int t, ssb): index(i), time(t), solved_sub_bombs(ssb) {};
    Bomb() = default;

    int top() const {
        if (this->index == 0) {
            // TODO
        }
    }
}

        std::pair<int, int> get_index_and_min(std::vector<int> vec) {
int min = INT_MAX;
int index = -1;
for (int i = 0 ; i < vec.size() ; i++) {
if (vec[i] >= 0 && vec[i] < min) {
min = vec[i];
index = i;
}
}
return std::make_pair(index, min);
}

void testcase() {
    int n; std::cin >> n; // # bombs
    std::vector<int> t(n); // Explosion times
    for (int i = 0 ; i < n ; i++) {
        std::cin >> t[i];
    }

    // Idea: "propagate" the minimal time, from the top to the bottom
    for (int i = 0 ; i <= LAST_STANDING ; i++) {
        int top = t[i];
        int bottom_left = t[(2 * i) + 1];
        int bottom_right = t[(2 * i) + 2];

        if (top < bottom_left) {
            t[(2 * i) + 1] = top - 1;
        }
        if (top < bottom_right) {
            t[(2 * i) + 2] = top - 1;
        }
    }

    std::vector<int> available(n, -1); // -1 <-> bomb is not in the queue
    for (int i = FIRST_GROUND ; i < n ; i++) {
        available[i] = t[i];
    }

    int current_time = 0;
    bool possible = true;
    for (int i = 0 ; i < n ; i++) {
        std::pair<int, int> index_and_min = get_index_and_min(available);
        int index = index_and_min.first;
        int t_min = index_and_min.second;

        if (t_min <= current_time) {
            // Already exploded
            possible = false;
            break;
        }

        available[index] = -2; // -2 <-> bomb is defused

        // Time to whip out my index magic y'all!
        if ((index - 1) % 2 == 0) {
            // This was a bottom-left bomb
            int bottom_right = index + 1;
            bool safe_to_climb = available[bottom_right] == -2;
            if (safe_to_climb) {
                int top = (index - 1) / 2;
                available[top] = t[top];
            }
        } else if ((index - 2) % 2 == 0) {
            // This was a bottom-right bomb
            int bottom_left = index - 1;
            bool safe_to_climb = available[bottom_left] == -2;
            if (safe_to_climb) {
                int top = (index - 2) / 2;
                available[top] = t[top];
            }
        }

        current_time++;
    }

    std::cout << (possible ? "yes" : "no") << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}