#include "bits/stdc++.h"
#include "math/polynomial.h"
#include "math/combinatorial.h"

using Mod = Modular<StaticModuloPolicy<998244353, unsigned int, MontgomeryModulo>>;
using Poly = Polynomial<Mod>;

int main() {
    int m;
    std::cin >> m;
    Combinatorial<Mod, Factorials> binom(1 << m);
    Poly dp = {1};
    Mod multiplier = pow(Mod(2), m);
    for (int i = 1; i < m; i++) {
        multiplier *= binom.factorial(1 << i);
    }
    for (size_t n = 2; n <= (1 << m); n *= 2) {
        Poly temp(n);
        for (size_t k = 0; k < n / 2; k++) {
            dp[k] *= binom.inverseFactorial(k) * binom.inverseFactorial(n / 2 - k - 1);
        }
        for (int i = 1; i <= n / 2; i++) {
            temp[i] = binom.inverseFactorial(i - 1) * binom.inverseFactorial(n / 2 - i);
        }
        dp = dp * temp;
        dp.resize(n);
        assert(dp[0] == 0);
        for (size_t j = 1; j < n; j++) {
            dp[j] *= binom.factorial(j - 1) * binom.factorial(n - j - 1);
        }
    }
    for (auto &x : dp) {
        std::cout << x * multiplier << "\n";
    }
    return 0;
}