#pragma once

#include <vector>
#include "math/convolution.h"

template<typename T>
class Polynomial : public std::vector<T> {
public:
    using std::vector<T>::vector;

    template<typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    Polynomial(U value) : std::vector<T>(1, value) {}

    Polynomial operator - () {
        Polynomial ans = *this;
        ans.apply([](auto &x) { return -x; });
        return ans;
    }

    Polynomial& operator *= (const Polynomial &other) {
        size_t m = this->size() + other.size() - 1;
        *this = SumConvolution<Polynomial, T>()(*this, other);
        this->resize(m);
        return *this;
    }

    friend Polynomial operator * (Polynomial lhs, const Polynomial &rhs) {
        return lhs *= rhs;
    }

    Polynomial& operator += (const Polynomial &other) {
        this->resize(std::max(this->size(), other.size()));
        for (int i = 0; i < this->size(); i++) {
            (*this)[i] += other[i];
        }
        return *this;
    }

    friend Polynomial operator + (Polynomial lhs, const Polynomial &rhs) {
        return lhs += rhs;
    }

    Polynomial& operator -= (const Polynomial &other) {
        this->resize(std::max(this->size(), other.size()));
        for (int i = 0; i < this->size(); i++) {
            (*this)[i] -= other[i];
        }
        return *this;
    }

    friend Polynomial operator - (Polynomial lhs, const Polynomial &rhs) {
        return lhs -= rhs;
    }

    template<typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    friend Polynomial operator * (U value, const Polynomial &poly) {
        Polynomial ans = poly;
        ans.apply([&](auto &x) {
            return T(value) * x;
        });
        return ans;
    }

    template<typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    friend Polynomial operator * (const Polynomial &poly, U value) {
        Polynomial ans = poly;
        ans.apply([&](auto &x) {
            return x * T(value);
        });
        return ans;
    }

    template<typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    friend Polynomial operator + (U value, const Polynomial &poly) {
        Polynomial ans = poly;
        ans.apply([&](auto &x) {
            return T(value) + x;
        });
        return ans;
    }

    template<typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    friend Polynomial operator + (const Polynomial &poly, U value) {
        Polynomial ans = poly;
        ans.apply([&](auto &x) {
            return x + T(value);
        });
        return ans;
    }

    template<typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    friend Polynomial operator - (U value, const Polynomial &poly) {
        Polynomial ans = poly;
        ans.apply([&](auto &x) {
            return T(value) - x;
        });
        return ans;
    }

    template<typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    friend Polynomial operator - (const Polynomial &poly, U value) {
        Polynomial ans = poly;
        ans.apply([&](auto &x) {
            return x - T(value);
        });
        return ans;
    }

private:
    void apply(auto&& scalarFunction) {
        for (auto &x : *this) {
            x = scalarFunction(x);
        }
    }
};