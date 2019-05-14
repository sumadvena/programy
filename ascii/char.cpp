#include <iostream>
#include <string>

void kon(std::string s) {
     for (int i{0}; i < s.length(); i++) {

        std::cout << " | " << s[i] << " = " << (int)s[i] << " | " << '\n'; // albo '\t'
    }
}

int main() {
    std::string konwertowany;
    //std::cout << "Program konwertuje pojedyncze znaki na odpowiedniki z tablicy ASCII\n";
    std::cout << "Wpisz tekst: ";
    std::getline(std::cin, konwertowany);
    kon(konwertowany);
    //std::cout << "Wprowadzony znak w tablicy ASCII ma wartość: " << '\n';
}