#pragma once

#include <vector>
#include "math/convolution.h"

template<typename T>
class Polynomial : public std::vector<T> {
public:
    using std::vector<T>::vector;

    Polynomial& operator *= (const Polynomial &other) {
        size_t m = this->size() + other.size() - 1;
        *this = SumConvolution<Polynomial, T>()(*this, other);
        this->resize(m);
        return *this;
    }

    friend Polynomial operator * (Polynomial lhs, const Polynomial &rhs) {
        return lhs *= rhs;
    }
};