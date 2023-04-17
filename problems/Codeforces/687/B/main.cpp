// https://codeforces.com/problemset/problem/687/B

#include "bits/stdc++.h"
#include "util/fastInput.h"

int main() {
    InputReader cin;
    long long n, k;
    cin >> n >> k;
    long long d = 1;
    for (int i = 0; i < n; i++) {
        long long x;
        cin >> x;
        d = std::gcd(d * x / std::gcd(d, x), k);
    }
    std::cout << (d == k ? "Yes" : "No") << "\n";
    return 0;
}