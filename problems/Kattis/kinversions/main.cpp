// https://open.kattis.com/problems/kinversions

#include "bits/stdc++.h"
#include "math/polynomial.h"
#include "util/io.h"

using Poly = Polynomial<std::complex<double>>;

int main() {
    auto &cin = std::cin;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::string s;
    cin >> s;
    size_t n = s.size();
    Poly a(n), b(n);
    for (size_t i = 0; i < n; i++) {
        if (s[i] == 'A') {
            a[i] = 1;
        } else {
            b[n - 1 - i] = 1;
        }
    }
    Poly res = a * b;
    for (size_t i = 1; i < n; i++) {
        std::cout << (long long) std::round(res[n - 1 + i].real()) << "\n";
    }
    return 0;
}