#include <iostream>

int main() {
    double val1, val2;
    std::cout << "Wpisz dwie liczby: ";
    std::cin >> val1 >> val2;

    if (val1 > val2) 
        std::cout << val1 << " > " << val2 << '\n';
    else 
        std::cout << val1 << " < " << val2 << '\n';

    std::cout << val1 + val2 << '\n'
        << val1 - val2 << '\n'
        << val1 * val2 << '\n'
        << val1 / val2 << '\n';
}