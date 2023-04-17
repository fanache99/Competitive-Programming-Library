//https://www.hackerrank.com/challenges/longest-increasing-subsequence-arrays/problem
#include "bits/stdc++.h"
#include "math/modular.h"

using Mod = Modular<StaticModuloPolicy<1000000007>>;

int main() {
    int tests;
    std::cin >> tests;
    for (int test = 1; test <= tests; test++) {
        int m, n;
        std::cin >> m >> n;
        Mod ans = 0;
        Mod pow1 = 1, pow2 = pow(Mod(n), m - n), bin = 1;
        for (int i = n; i <= m; i++) {
            ans += pow1 * pow2 * bin;
            pow1 *= n - 1;
            pow2 /= n;
            bin = bin * i / (i - n + 1);
        }
        std::cout << ans << "\n";
    }
    return 0;
}