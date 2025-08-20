// https://judge.yosupo.jp/problem/matrix_det
#include <string>
#include "util/io.h"
#include "math/modular.h"
#include "math/matrix.h"

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

using Mod = Modular<StaticModuloPolicy<998244353, unsigned int, MontgomeryModulo>>;

int main() {
    size_t n;
    cin >> n;
    Matrix<Mod> a(n, n);
    cin >> a;
    std::cout << a.determinant();
    return 0;
}