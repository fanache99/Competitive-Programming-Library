// https://codeforces.com/contest/1117/problem/D
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

#include "math/modular.h"
#include "math/matrix.h"

using Mod = Modular<StaticModuloPolicy<1000000007, unsigned int, MontgomeryModulo>>;

int main() {
    long long n;
    size_t m;
    cin >> n >> m;
    if (n < m) {
        std::cout << "1\n";
        return 0;
    }
    Matrix<Mod> base(m, m);
    for (size_t i = 0; i < m - 1; i++) {
        base[i][i + 1] = 1;
    }
    base[m - 1][0] = base[m - 1][m - 1] = 1;
    Vector<Mod> init({m}, 1);
    auto power = pow(base, n - m + 1);
    auto ans = power * init;
    std::cout << ans[m - 1] << "\n";
    return 0;
}