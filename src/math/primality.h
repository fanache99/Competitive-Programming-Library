#pragma once

#include "math/modular.h"

template<typename T>
T primeOrFactor(T n) {
    using Mod = Modular<DynamicModuloPolicy<std::make_unsigned_t<T>, MontgomeryModulo>>;
    using UT = std::make_unsigned_t<T>;
    if (n == 2) {
        return 1;
    }
    if (n % 2 == 0) {
        return n / 2;
    }
    auto _ = Mod::changeMod(n);
    unsigned long long as[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    Mod one(1), minusOne(-1);
    T d = n - 1, s = 0;
    while (d % 2 == 0) {
        d /= 2, s++;
    }
    for (auto a : as) {
        auto p = pow(Mod(a), d);
        T i = s;
        while (p != one && p != minusOne && a % n && i--) {
            auto q = p * p;
            if (q == one) {
                return std::gcd(UT(p) - 1, n);
            }
            p = q;
        }
        if (p != minusOne && i != s) {
            return 0;
        }
    }
    return 1;
}

template<typename T>
constexpr bool isPrime(T n) {
    return primeOrFactor(n) == 1;
}

template<typename T>
std::vector<T> factor(T n) {
    using UT = std::make_unsigned_t<T>;
    using Mod = Modular<DynamicModuloPolicy<UT, MontgomeryModulo>>;

    auto getFactor = [&](T n) -> T {
        auto _ = Mod::changeMod(n);
        constexpr T m = std::numeric_limits<T>::digits;
        T r = 1, g;
        Mod c(0), y, q, x, ys;
        do {
            ++c;
            y = 2;
            q = 1;
            g = 1;
            do {
                x = y;
                for (T i = 0; i < r; i++) {
                    y = y * y + c;
                }
                ys = y;
                for (T i = 0; i < r; i++) {
                    y = y * y + c;
                    q *= y - x;
                    if ((i + 1) % m == 0) {
                        g = std::gcd(UT(q), n);
                        if (g != 1) {
                            break;
                        }
                        ys = y;
                    }
                }
                if (g == 1 && r % m != 0) {
                    g = std::gcd(UT(q), n);
                }
                r *= 2;
            } while (g == 1);
            if (g == n) {
                do {
                    ys = ys * ys + c;
                    g = std::gcd(UT(ys - x), n);
                } while (g == 1);
            }
        } while (g == n);
        return g;
    };

    if (n == 1) {
        return {};
    }
    T d = primeOrFactor(n);
    if (d == 1) {
        return {n};
    }
    if (d == 0) {
        d = getFactor(n);
    }
    auto divs1 = factor(d), divs2 = factor(n / d);
    divs1.insert(divs1.end(), divs2.begin(), divs2.end());
    return divs1;

}
