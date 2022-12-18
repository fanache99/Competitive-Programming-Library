#pragma once

#include "math/modular.h"

template<typename T>
constexpr bool isPrime(T n) {
    using Mod = Modular<MemberModulo<T>>;
    if (n == 2) {
        return true;
    }
    constexpr unsigned long long as[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    T d = n - 1, s = 0;
    while (d % 2 == 0) {
        d /= 2, s++;
    }
    for (auto a : as) {
        auto p = pow(Mod(a, n), d);
        T i = s;
        while (p != Mod(1, n) && p != Mod(-1, n) && a % n && i--) {
            p *= p;
        }
        if (p != Mod(-1, n) && i != s) {
            return false;
        }
    }
    return true;
}

template<typename T>
constexpr std::vector<T> factor(T n) {
    using Mod = Modular<MemberModulo<T>>;
    if (n == 1) {
        return {};
    }
    if (isPrime(n)) {
        return {n};
    }
    auto g = [&](auto x) {
        return x * x + Mod(1, n);
    };
    Mod x(0, n), y(0, n);
    T d = 1, i = 1;
    while (d == 1) {
        if (x == y) {
            x = Mod(++i, n), y = g(x);
        }
        d = std::gcd(n, std::max(T(x), T(y)) - std::min(T(x), T(y)));
        x = g(x), y = g(g(y));
    }
    auto divs1 = factor(d), divs2 = factor(n / d);
    divs1.insert(divs1.end(), divs2.begin(), divs2.end());
    return divs1;
}
