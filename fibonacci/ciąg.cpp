//niedopracowane

#include <iostream>

int main() {
    int przedostatnia = 0, ostatnia = 1, wynik;
    std::cout << przedostatnia << " " << ostatnia << " ";
    do {
        wynik = przedostatnia + ostatnia;
        przedostatnia = ostatnia;
        ostatnia= wynik;
        std::cout << wynik << " ";
    } while (wynik <= 1516303490); // dalej są minusowe liczby
    std::cout << '\n';
}