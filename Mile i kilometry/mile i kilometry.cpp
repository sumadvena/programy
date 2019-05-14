#include <iostream>

double dist; //dystans

void choice() {
	int x; // wybór
	do {
		std::cin >> x;
		if (x != 1 && x != 2) {
			std::cout << "Zły wybór, wpisz jeszcze raz: ";
		}
		else {
			switch (x) {
				case 1:
					std::cout << dist << "km, to ";
					dist *= 0.621371192;
					std::cout << dist <<" mil\n";
					break;
				case 2:
					std::cout << dist << "mil, to ";
					dist *= 1.609344;
					std::cout << dist << " km\n";
					break;
				default:
					choice();
					break;
				}
			}
	} while (x != 1 && x != 2);
}

void writeDistance() {
	do {
		std::cin >> dist;
		if (dist <= 0) {
			std::cout << "Zła odległość, wpisz jeszcze raz: ";
			writeDistance();
		}
		else {
			std::cout << '\n' << "Co chcesz przeliczyć?\n" << '\n' 
				<< "Kilometry na mile - wpisz 1.\n" << "Mile na kilometry - wpisz 2.\n" << '\n' 
				<< "Twój wybór: ";
		}
	} while (dist <= 0);
}

int main() {
	std::cout << "Witaj w progrogramie przeliczającym odległości.\n" 
		<< "Chcesz przeliczyć mile na kilometry, czy kilometry na mile?\n";
	std::cout << '\n' << "Wpisz odległość: ";
	writeDistance();
	choice();
}