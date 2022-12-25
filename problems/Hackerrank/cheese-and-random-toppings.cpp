// https://www.hackerrank.com/challenges/cheese-and-random-toppings/problem

#include "bits/stdc++.h"
#include "math/modular.h"
#include "math/combinatorial.h"
#include "math/primality.h"

using Mod = Modular<DynamicModuloPolicy<>>;
using VarMod = VarModular<DynamicModuloPolicy<>>;

VarMod lukas(int n, int k, int mod) {
    auto _ = Mod::changeMod(mod);
    Combinatorial<Mod, Factorials> c(mod - 1);
    Mod ans = 1;
    while (n != 0 || k != 0) {
        ans *= c.binomial(n % mod, k % mod);
        n /= mod, k /= mod;
    }
    return VarMod(ans);
}

int main() {
    int tests;
    std::cin >> tests;
    for (int test = 1; test <= tests; test++) {
        int n, k, mod;
        std::cin >> n >> k >> mod;
        auto divs = factor(mod);
        std::vector<VarMod> binomials;
        std::transform(divs.begin(), divs.end(), std::back_inserter(binomials), [&] (int d) {
            return lukas(n, k, d);
        });
        std::cout << VarMod(binomials) << "\n";
    }
    return 0;
}