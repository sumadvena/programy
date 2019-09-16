#include <iostream>
#include <fstream>

int main() {

    std::ofstream f;
    f.open ("fi.txt");

    uint_fast64_t przedostatnia{0}, ostatnia{1}, wynik;
    std::cout << przedostatnia << '\n' << ostatnia << '\n';
    
    while (true) {
        wynik = przedostatnia + ostatnia;
        przedostatnia = ostatnia;
        ostatnia = wynik;
        std::cout << '\n' << wynik << '\n';
        f << '\n' << wynik << '\n';
    }
    std::cout << '\n';
}
