#include <iostream>
#include <array>
#include <algorithm>

int main() {
    std::array<double, 3> numbers;
    int i{};
    for (i; i < 3; i++) {
        std::cout << "Wpisz liczbę: ";
        std::cin >> numbers[i];
    }
    
    std::sort (numbers.begin(), numbers.end());
    std::cout << "Posortowane dane: ";
    for (i = 0; i < 3; i++) {
        std::cout << numbers[i] << ", ";
    }
    std::cout << '\n';
}