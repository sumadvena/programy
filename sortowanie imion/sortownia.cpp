#include <iostream>
#include <array>
#include <string>
#include <algorithm>

int main() {
    std::array<std::string, 3> names; // tablica, w której będą przechowywane imiona
    int i{};
     for (i; i < 3; i++) { // pętla wczytująca imiona
        std::cout << "Wpisz imię: " << names[i];
        std::cin >> names[i];
    }
    std::sort (names.begin(), names.end()); // sortowanie
    std::cout << "Posortowane dane: ";
    for (i = 0; i < 3; i++) {
        std::cout << names[i] << ", ";
    }
    std::cout << '\n';
}