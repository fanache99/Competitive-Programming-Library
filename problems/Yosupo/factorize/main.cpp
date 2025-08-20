#include "bits/stdc++.h"
#include "math/primality.h"
#include "util/timer.h"

int main() {
    Timer t;
    int tests;
    std::cin >> tests;
    for (int test = 1; test <= tests; test++) {
        long long n;
        std::cin >> n;
        auto divs = factor(n);
        sort(divs.begin(), divs.end());
        std::cout << divs.size();
        for (auto d : divs) {
            std::cout << " " << d;
        }
        std::cout << "\n";
    }
    std::cout << t.elapsed();
    return 0;
}
