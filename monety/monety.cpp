#include <iostream>

int main() {
    double grosz1, grosz2, grosz5, grosz10, grosz20, grosz50, zl;
    std::cout << "Ilość jednogroszówek: ";
    std::cin >> grosz1;
    std::cout << "Ilość dwugroszówek: ";
    std::cin >> grosz2;
    std::cout << "Ilość pięciogroszówek: ";
    std::cin >> grosz5;
    std::cout << "Ilość dziesięciogroszówek: ";
    std::cin >> grosz10;
    std::cout << "Ilość dwudziestogroszówek: ";
    std::cin >> grosz20;
    std::cout << "Ilość pięćdźesięciogroszówek: ";
    std::cin >> grosz50;
    std::cout << "Ilość jednozłotówek: ";
    std::cin >> zl;
    double wartz = (grosz1 * 0.01) + (grosz2 * 0.02) + (grosz5 * 0.05)
         + (grosz10 * 0.1) + (grosz20 * 0.2) + (grosz50 * 0.5) + zl; // wartość monet wyrażona w złotówkach
    double wartg = wartz * 100; // wartość monet wyrażona w groszach
    std::cout << "Wartość monet w złotówkach wynosi: " << wartz << " zł\n"
        << "Wartość monet w groszach wynosi: " << wartg << " gr\n";

}