// nieskończone

#include <iostream>
#include <vector>
#include <istream>

int main() {
    int num;
    std::vector<int> nums;

    while (std::cin >> num) {
        int i{};
        char c = std::cin.peek();

        //std::cout << "Wpisz dwie liczby: ";
        nums.push_back(num);
        std::cout << nums.size();
        i++;

        if (c == '|')
            break;
    }
}