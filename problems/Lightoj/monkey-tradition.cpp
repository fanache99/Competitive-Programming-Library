// https://lightoj.com/problem/monkey-tradition

#include "bits/stdc++.h"
#include "math/modular.h"

using Mod = VarModular<DynamicModuloPolicy<uint64_t>>;

int main() {
    int tests;
    std::cin >> tests;
    for (int test = 1; test <= tests; test++) {
        std::cout << "Case " << test << ": ";
        try {
            int n;
            std::cin >> n;
            std::vector<Mod> v;
            for (int i = 0; i < n; i++) {
                int p, r;
                std::cin >> p >> r;
                v.emplace_back(r, p);
            }
            std::cout << Mod(v) << "\n";
        } catch (Mod::NoSolution&) {
            std::cout << "Impossible\n";
        }
    }
    return 0;
}