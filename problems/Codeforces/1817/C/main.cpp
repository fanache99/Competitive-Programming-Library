#pragma GCC optimize("Ofast")

#include <string>
#include "util/io.h"

#ifdef LOCAL_PROJECT

std::string input_path() {
    std::string file = __FILE__;
    std::string directory = file.substr(0, file.rfind('/'));
    return directory + "/input.txt";
}

FileReader cin(input_path());
#else
StdinReader cin;
#endif

#include "math/polynomial.h"
#include "math/modular.h"

using Mod = Modular<StaticModuloPolicy<1000000007, unsigned int, MontgomeryModulo>>;

int main() {
    int d;
    cin >> d;
    Polynomial<Mod> a(d + 1), b(d + 1);
//    a.resize(b + 1);
//    b.resize(b + 1);
    cin >> a >> b;
    std::cout << a << b;
    std::cout << a * b;
    return 0;
}