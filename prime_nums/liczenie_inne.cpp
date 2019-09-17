#include <iostream>
#include <fstream>
#include <vector>

void liczonko (uint_fast64_t zakres_beg, uint_fast64_t zakres_end) {
    std::ofstream primes;
    primes.open ("pnums.txt");
    std::ofstream perfects;
    std::vector<uint_fast64_t> dzielniki;
    uint_fast64_t liczba = zakres_beg;

    while (zakres_beg <= zakres_end) {
        liczba = zakres_beg;
        std::cout << "[ " << liczba << " ]: ";

        for (int i{1}; i <= liczba; i++) {
            if (liczba % i == 0) {
                std::cout << ' ' << i << ' ';
                dzielniki.push_back(i);
            }
        }
        if (dzielniki.size() == 2) {
                primes << '\n' << liczba << '\n';
        }

        dzielniki.clear();
        std::cout << '\n';
        zakres_beg++;
    }
    primes.close();
}

int main() {
    uint_fast64_t zakres_beg, zakres_end;
    std::cout << "Wpisz zakres liczb (początek i koniec oddziel spacją): ";
    std::cin >> zakres_beg >> zakres_end;
    liczonko(zakres_beg, zakres_end);
}
