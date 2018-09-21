#include <iostream>

int main() {
    int num;
    std::cout << "Wpisz liczbę: ";
    std::cin >> num;
    if (num % 2 == 0)
        std::cout << "\nLiczba jest parzysta!\n";
    else    
        std::cout << "\nLiczba nie jest parzysta!\n";
}