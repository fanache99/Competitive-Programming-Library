#pragma once

#include "util/typeTraits.h"
#include "math/basic.h"

template<typename T>
class Factorials {
protected:
    explicit Factorials(int n) : factorials(n + 1), inverseFactorials(n + 1) {
        factorials[0] = inverseFactorials[0] = 1;
        for (int i = 1; i <= n; i++) {
            factorials[i] = factorials[i - 1] * i;
        }
        inverseFactorials[n] = 1 / factorials[n];
        for (int i = n - 1; i > 0; i--) {
            inverseFactorials[i] = inverseFactorials[i + 1] * (i + 1);
        }
    }

    std::vector<T> factorials, inverseFactorials;
};

template<typename T>
class BinomialCoefficients {
protected:
    explicit BinomialCoefficients(int n) : binomials(n + 1) {
        for (int i = 0; i <= n; i++) {
            binomials[i].resize(i + 1);
            binomials[i][0] = binomials[i][i] = 1;
            for (int j = 1; j < i; j++) {
                binomials[i][j] = binomials[i - 1][j] + binomials[i - 1][j - 1];
            }
        }
    }

    std::vector<std::vector<T>> binomials;
};

template<typename T, template<typename> typename ...Memoizations>
class Combinatorial : Memoizations<T>... {
public:
    explicit Combinatorial(int limit_) : Memoizations<T>(limit_)..., limit(limit_) {}

    T factorial(int n) {
        assert(n <= limit);
        if constexpr (typeInList<Factorials, Memoizations...>()) {
            return this->factorials[n];
        } else {
            return n == 0 ? 1 : n * factorial(n - 1);
        }
    }

    T binomial(int n, int k) {
        if (n < 0 || k < 0 || n < k) {
            return 0;
        }
        assert(n <= limit);
        if constexpr (typeInList<BinomialCoefficients, Memoizations...>()) {
            return this->binomials[n][k];
        } else {
            return factorial(n) / (factorial(k) * factorial(n - k));
        }
    }

private:
    int limit;
};