#include <iostream>

int main() {

    int number_of_words{0};
    std::string previous = " ";
    std::string current;

    std::cout << "Pisz:\n";

    while (std::cin >> current) {
        ++number_of_words;
        if (previous == current)
          std::cout << "Słowo numer " << number_of_words << " powtarza się: " << current << '\n';
        previous = current;
    }
}