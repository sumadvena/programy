#include <iostream>
#include <fstream>
#include "../c17/Cbignum.h"
#include "../c17/Cbignumf.h"
#include "../c17/Cthr.h"
#include "../c17/Cios.h"
#include "../c17/Cbignums.h"
#include "../c17/Cbnl.h"
#include "../c17/Exarray.h"
#include "../c17/Exdebug.h"
#include "../c17/Exthread.h"

int main() {
    system("ulimt");

    std::ofstream f;
    f.open ("fi.txt");

    cBigNumber przedostatnia{0}, ostatnia{1}, wynik;
    std::cout << przedostatnia << '\n' << ostatnia << '\n';
    do {
        wynik = przedostatnia + ostatnia;
        przedostatnia = ostatnia;
        ostatnia = wynik;
        std::cout << '\n' << wynik << '\n';
        f << '\n' << wynik << '\n';
    } while (true);
    std::cout << '\n';
}
