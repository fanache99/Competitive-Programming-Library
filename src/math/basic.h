#pragma once

#include <complex>

template<typename T, typename P, typename Op>
constexpr T fastOp(T base, P power, T e, Op op) {
    T answer = e;
    for (; power; base = op(base, base), power >>= 1) {
        if (power & 1) {
            answer = op(answer, base);
        }
    }
    return answer;
}

template<typename T, typename P>
constexpr T pow(T base, P power) {
    return fastOp<T, P>(base, power, 1, [](T a, T b) { return a * b; });
}

template<typename T>
constexpr std::tuple<T, T, T> euclid(T a, T b) {
    if (b) {
        auto [x, y, d] = euclid(b, a % b);
        return {y, x - a / b * y, d};
    } else {
        return {1, 0, a};
    }
}

// Simulate partial template specialization for functions.
namespace detail {
    template<typename T>
    struct UnityRoot {};

    template<typename T>
    struct UnityRoot<std::complex<T>> {
        static constexpr std::complex<T> impl(int n) {
            return std::polar(T(1), T(2 * M_PI / n));
        }
    };
}

template<typename T>
constexpr T unityRoot(int n) {
    return detail::UnityRoot<T>::impl(n);
}