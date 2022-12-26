#pragma once

#include "math/basic.h"
#include "math/modular.h"

template<template<typename> typename Container, typename T>
class ConvolutionBase {
protected:
    using Vec = Container<T>;

public:
    Vec operator () (Vec a, Vec b) {
        size_t n = minSize(a.size(), b.size());
        a.resize(n), b.resize(n);
        DFT(a), DFT(b);
        for (size_t i = 0; i < n; i++) {
            a[i] *= b[i];
        }
        DFTInv(a);
        return a;
    }

private:
    virtual size_t minSize(size_t na, size_t nb) = 0;

    virtual void DFT(Vec &a) = 0;

    virtual void DFTInv(Vec &a) = 0;
};

template<template<typename> typename Container, typename T>
class SumConvolution : public ConvolutionBase<Container, T> {
private:
    using Vec = typename ConvolutionBase<Container, T>::Vec;

    size_t minSize(size_t na, size_t nb) override {
        size_t n = na + nb;
        while (!isPower2(n)) {
            n++;
        }
        return n;
    }

    void DFT(Vec &a) override {
        return DFTImpl(a, false);
    }

    void DFTInv(Vec &a) override {
        return DFTImpl(a, true);
    }

    void DFTImpl(Vec &a, bool inv) {
        auto temp = a;
        size_t n = a.size();
        assert(isPower2(n));
        for (size_t step = n / 2; step > 0; step /= 2) {
            T w = unityRoot<T>(n / step), wn = 1;
            if (inv) {
                w = 1 / w;
            }
            for (size_t i = 0; i < n / 2; i += step, wn *= w) {
                for (size_t j = 0; j < step; j++) {
                    T u = a[2 * i + j], v = wn * a[2 * i + j + step];
                    temp[i + j] = u + v;
                    temp[i + n / 2 + j] = u - v;
                }
            }
            swap(a, temp);
        }
        if (inv) {
            T divide = T(1) / n;
            for (auto &x : a) {
                x *= divide;
            }
        }
    }
};