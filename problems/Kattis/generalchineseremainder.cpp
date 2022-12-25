// https://open.kattis.com/problems/generalchineseremainder

#include "bits/stdc++.h"
#include "math/modular.h"

using Mod = VarModular<DynamicModuloPolicy<uint64_t>>;

int main() {
    int tests;
    std::cin >> tests;
    for (int test = 1; test <= tests; test++) {
        long long a1, n1, a2, n2;
        std::cin >> a1 >> n1 >> a2 >> n2;
        try {
            Mod ans({Mod(a1, n1), Mod(a2, n2)});
            std::cout << ans << " " << ans.modulo() << "\n";
        } catch (Mod::NoSolution&) {
            std::cout << "no solution\n";
        }
    }
    return 0;
}