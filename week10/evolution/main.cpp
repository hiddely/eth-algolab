//
// Created by Hidde Lycklama on 11/25/18.
//

#include <iostream>
#include <unordered_map>
#include <string>
#include <set>
#include <algorithm>
#include <vector>

typedef int NameIndex;

struct Entry {
    int i;
    int age;
    NameIndex name;
    NameIndex parent = -1;

    Entry() = default;
    Entry(int zz, int a, NameIndex b): i(zz), age(a), name(b) {}

    bool operator < (const Entry &e) const {
        return this->age < e.age && this->name < e.name;
    }
};
//typedef std::map<int, Entry> Lookup;

struct APtrComp
{
    bool operator()(const Entry* lhs, const Entry* rhs) const  {
        if (lhs->age == rhs->age && lhs->name == rhs->name) {
            return lhs->i < rhs->i;
        }
        if (lhs->age == rhs->age) {
            return lhs->name < rhs->name;
        }
        return lhs->age < rhs->age;
    }
};

typedef std::vector<Entry*> Lookup;


void testcase() {
    int n, q;
    std::cin >> n >> q;

    std::vector<std::string> names(n);
    std::unordered_map<std::string, NameIndex> nameLookup(n);
    std::vector<int> ages(n);
    std::vector<bool> leafs(n, true);
    std::vector<NameIndex> ancestors(n, -1);
    for (int i = 0; i < n; i++) {
        std::string name; std::cin >> name;
        int age; std::cin >> age;
        names[i] = name;
        nameLookup[name] = i;
        ages[i] = age;
    }

    for (int i = 0; i < n - 1; i++) {
        std::string child; std::cin >> child;
        std::string parent; std::cin >> parent;
        NameIndex par = nameLookup[parent];
        NameIndex ch = nameLookup[child];
        ancestors[ch] = par;
        leafs[par] = false;
    }
    std::vector<Entry> challengeOrder(n);

    std::vector<std::vector<Entry *>> challenges(n);
    std::vector<bool> found(n);
//    std::set<Entry*> sortedChallenges;
//std::cerr << "1" << std::endl;
    for (int i = 0; i < q; i++) {
        std::string current; std::cin >> current;
        int maxAge; std::cin >> maxAge;
        NameIndex cIndex = nameLookup[current];
        challengeOrder[i] = Entry(i, maxAge, cIndex);

        challenges[cIndex].push_back(&challengeOrder[i]);
//        sortedChallenges.insert(&challengeOrder[i]);
    }

//    std::cerr << "2" << std::endl;

    for (int i = 0; i < n; i++) {
        if (leafs[i]) {
            NameIndex current = i;

//            std::cerr << "5" << std::endl;

            Lookup sortedChallenges;
            while (current != -1) {
                if (!found[current]) {
//                    if (current == "aaf") {
//                        for (auto dd = sortedChallenges.begin(); dd != sortedChallenges.end(); dd++) {
//                            std::cout << current << ": " << (*dd)->name << " " << (*dd)->age << std::endl;
//                        }
//                        std::cout << " " << std::endl;
//
//                        for (auto dd = challenges[current].begin(); dd != challenges[current].end(); dd++) {
//                            std::cout << current << ": " << (*dd)->name << " " << (*dd)->age << std::endl;
//                            sortedChallenges.insert(*dd);
//                        }
//                        std::cout << " " << std::endl;
//
//                    }
                    sortedChallenges.insert(sortedChallenges.end(), challenges[current].begin(), challenges[current].end());

                    found[current] = true;
//                    if (current == "aaf") {
////                        std::cout << "Adding aff " << found[current] << std::endl;
//                        for (auto dd = sortedChallenges.begin(); dd != sortedChallenges.end(); dd++) {
//                            std::cout << current << ": " << (*dd)->name << " " << (*dd)->age << std::endl;
//                        }
//                    }
                } else {
                    break;
                }
                NameIndex parent = ancestors[current];
                current = parent;
            }

            std::sort(sortedChallenges.begin(), sortedChallenges.end(), APtrComp());
//                for (auto dd = sortedChallenges.begin(); dd != sortedChallenges.end(); dd++) {
//                    std::string n = (*dd)->name;
//                    if (n == "aaf") {
//                        std::cout << current << ": " << n << std::endl;
//                    }
//                }

//                std::cerr << "7 " << current << std::endl;
            current = i;
            Lookup::iterator iter = sortedChallenges.begin();
            while (current != -1 && iter != sortedChallenges.end()) {

                NameIndex parent = ancestors[current];
//                std::cout << "Age: " << e.age << std::endl;
//                std::cout << (*max) << (*sortedChallenges.end()) << std::endl;

                for (; iter != sortedChallenges.end(); iter++) {
//                    if ((*iter)->name == "aaf") {
//                        std::cout << "At " << (*iter)->name << " " << current << std::endl;
//                    }
                    if (parent != -1 && ages[parent] <= (*iter)->age) {
                        break;
                    }
//                    std::cerr << "Setting " << names[(*iter)->name] << ": " << current << std::endl;
                    (*iter)->parent = current;
//                    std::cout << "Erasing " << (*iter)->name << std::endl;
//                    iter = sortedChallenges.erase(iter);
                }
                current = parent;
            }

//            std::cerr << "6" << std::endl;

            // find challenges

        }
    }

//    std::cerr << "3" << std::endl;

    bool first = true;
    for (int i = 0; i < q; i++) {
        if (!first) {
            std::cout << " ";
        } else {
            first = false;
        }
        std::cout << names[challengeOrder[i].parent];
        if (challengeOrder[i].parent == -1) {
            std::cout << "EMPTY " << std::endl;
            std::cout << challengeOrder[i].name << std::endl;
            std::cout << challengeOrder[i].age << std::endl;
        }
    }

//    std::cerr << "4" << std::endl;

    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) {
        testcase();
    }
    return 0;
}