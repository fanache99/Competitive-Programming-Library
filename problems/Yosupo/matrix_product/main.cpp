// https://judge.yosupo.jp/problem/matrix_product

#include "bits/stdc++.h"
#include "math/polynomial.h"
#include "util/io.h"
#include "math/matrix.h"

using Mod = Modular<StaticModuloPolicy<998244353, unsigned int, MontgomeryModulo>>;
using T = Mod;

int main() {
    auto& cin = std::cin;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    size_t n, m, p;
    cin >> n >> m >> p;
    Matrix<Mod> a(n, m), b(m, p);
    std::cin >> a >> b;
    Matrix<Mod> c = a * b;
    std::cout << c;
    return 0;
}
