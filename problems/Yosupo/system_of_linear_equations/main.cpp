// https://judge.yosupo.jp/problem/system_of_linear_equations
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
    size_t n, m;
    cin >> n >> m;
    Matrix<Mod> a(n, m);
    Vector<Mod> b(n);
    cin >> a >> b;
    auto x = solve(a, b);
    if (!x) {
        std::cout << "-1\n";
        return 0;
    }
    auto& sol = *x;
    Matrix<Mod> temp(n, m + 1);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            temp[i][j] = a[i][j];
        }
    }
    auto [pivots, _] = temp.makeRREF();
    std::cout << m - pivots.size() << "\n";
    std::cout << sol;
    std::vector<Vector<Mod>> bases;
    std::vector<bool> isPivot(m);
    for (auto pivot : pivots) {
        isPivot[pivot] = true;
    }
    for (size_t j = 0; j < m; j++) {
        if (!isPivot[j]) {
            Vector<Mod> base(m);
            base[j] = 1;
            for (size_t i = 0; i < pivots.size(); i++) {
                base[pivots[i]] = (temp(i, m) - temp(i, j))/ temp(i, pivots[i]);
            }
            bases.push_back(base);
        }
    }
    std::cout << bases;
    return 0;
}