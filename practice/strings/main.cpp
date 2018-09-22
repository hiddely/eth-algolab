//
// Created by Hidde Lycklama on 9/21/18.
//

#include <iostream>
#include <string>

int main() {
    std::cout << "Hello, World!" << std::endl;
    int n; std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::string s1; std::cin >> s1;
        std::string s2; std::cin >> s2;
        std::string line = std::to_string(s1.length()) + " " + std::to_string(s2.length());
        std::cout << line << std::endl;
        std::cout << s1 << s2 << std::endl;
        std::reverse(s1.begin(), s1.end());
        std::reverse(s2.begin(), s2.end());
        char temp = s1[0];
        s1[0] = s2[0];
        s2[0] = temp;
        std::cout << s1 << " " << s2 << std::endl;
    }
    return 0;
}