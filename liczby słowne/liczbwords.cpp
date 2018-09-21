#include <iostream>

int main() {
    int num;
    std::cout << "Wpisz liczbę od 0, do 4: ";
    std::cin >> num;
    if (num == 0)
        std::cout << num << " = zero\n";
    if (num == 1)
        std::cout << num << " = jeden\n";
    if (num == 2)
        std::cout << num << " = dwa\n";
    if (num == 3)
        std::cout << num << " = trzy\n";
    if (num == 4)
        std::cout << num << " = cztery\n";
    else 
        num--;
        std::cout << "Wpisane znaki nie są obsługiwane przez program!\n";
}