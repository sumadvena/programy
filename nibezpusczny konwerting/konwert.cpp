#include <iostream>

int main() {
    double d;
    
    std::cout << "Wpisz liczbę: \n";

    while (std::cin >> d) {
        int i = d;
        char c = i;
        int i2 = c;
        std::cout << "d == " << d
         << " i == " << i
         << " i2 == " << i2
         << " char( " << c << " )\n";
    }
}