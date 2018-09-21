#include <iostream>
#include <string>

int main() {

    string previous = " ";
    string current;

    std::cout << "Pisz zdania: \n";

    while (std::cin >> current) {
        if (previous == current) {
            std::cout << "Powtórzone słowo: " << current << '\n';
        }
        previous = current;
    }
}