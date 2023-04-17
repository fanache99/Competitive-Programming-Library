// https://judge.yosupo.jp/problem/convolution_mod

#include "bits/stdc++.h"
#include "math/polynomial.h"
#include "util/io.h"

using Mod = Modular<StaticModuloPolicy<998244353, unsigned int, MontgomeryModulo>>;
using Poly = Polynomial<Mod>;

int main() {
    auto& cin = std::cin;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    size_t n, m;
    cin >> n >> m;
    Poly a(n), b(m);
    for (size_t i = 0; i < n; i++) {
        cin >> a[i];
    }
    for (size_t i = 0; i < m; i++) {
        cin >> b[i];
    }
    Poly c = a * b;
    for (auto &x : c) {
        std::cout << x << " ";
    }
    return 0;
}
