#include "bits/stdc++.h"
#include "math/modular.h"
#include "math/combinatorial.h"

using Mod = Modular<StaticModuloPolicy<1000000007>>;

int main() {
    int a, b, n;
    std::cin >> a >> b >> n;
    Combinatorial<Mod, Factorials> c(n);

    auto isGood = [&](int x) {
        while (x) {
            if (x % 10 != a && x % 10 != b) {
                return false;
            }
            x /= 10;
        }
        return true;
    };

    Mod ans(0);
    for (int i = 0; i <= n; i++) {
        int x = i * a + (n - i) * b;
        if (isGood(x)) {
            ans += c.binomial(n, i);
        }
    }
    std::cout << ans << "\n";
    return 0;
}