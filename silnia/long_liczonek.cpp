#include <iostream>

int main() {
    int silnik{};   // silnik silni
    uint_fast64_t wynik{1};
    std::cout << "Wprowadź liczbę, której silnia zostanie obliczona: ";
    std::cin >> silnik;

    for (int i{1}; i <= silnik; i++) {
        wynik *= i;
    }

    std::cout << silnik << "! = " << wynik << '\n';
}
