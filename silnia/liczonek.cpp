#include <iostream>

int main() {
    int silnik{}, wynik{1};   // silnik silni
    std::cout << "Wprowadź liczbę, której silnia zostanie obliczona: ";
    std::cin >> silnik;

    for (int i{1}; i <= silnik; i++) {
        wynik *= i;
    }

    std::cout << silnik << "! = " << wynik << '\n';
}
