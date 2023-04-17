#include "catch2/catch_test_macros.hpp"
#include "math/matrix.h"
#include "util/timer.h"
#include "math/modular.h"
#include <iostream>

using Mod = Modular<StaticModuloPolicy<998244353, unsigned int, MontgomeryModulo>>;

using T = int;
constexpr int SIZE = 1000;

void func() {
    const Matrix<T> m(2, 3);
    const Matrix<T> mm(2, 3);
    auto x = m(1, 2);
//    MatrixRef<const T> ref(m.shape, m.data);
    auto y = m[1];
    Matrix<T> n = m;
    Matrix<T> p = n;
    p = std::move(n);
}

TEST_CASE("Modulo Implementations") {
    srand(time(0));


    int n = SIZE, m = SIZE, p = SIZE;
    Matrix<T> a(n, m), b(m, p), c(n, p);
//    std::vector<std::vector<T>> a(n, std::vector<T>(m));
//    std::vector<std::vector<T>> b(m, std::vector<T>(p));
//    std::vector<std::vector<T>> c(n, std::vector<T>(p));

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            a[i][j] = rand();
        }
    }
    for (size_t j = 0; j < m; j++) {
        for (size_t k = 0; k < p; k++) {
            b[j][k] = rand();
        }
    }
    std::vector<std::vector<T>> bt(p, std::vector<T>(m));
    for (int j = 0; j < m; j++) {
        for (int k = 0; k < p; k++) {
            bt[k][j] = b[j][k];
        }
    }

    std::cout << "Starting\n";
    Timer t;
//    for (size_t i = 0; i < n; i++) {
////        std::cout << "Done " << i << " total " << t.elapsed() << "ms\n";
//        for (size_t j = 0; j < m; j++) {
//            for (size_t k = 0; k < p; k++) {
//                c[i][k] += a[i][j] * b[j][k];
//            }
//        }
//    }
//    for (size_t i = 0; i < n; i++) {
//        std::cout << "Done " << i << " total " << t.elapsed() << "ms\n";
//        for (size_t j = 0; j < m; j++) {
//            for (size_t k = 0; k < p; k++) {
//                c(i, k) += a(i, j) * b(j, k);
//            }
//        }
//    }
//    for (size_t i = 0; i < n; i++) {
//        std::cout << "Done " << i << " total " << t.elapsed() << "ms\n";
//        for (size_t j = 0; j < m; j++) {
//            for (size_t k = 0; k < p; k++) {
//                c.data[c.shape.start + c.shape.strides[0] * i + c.shape.strides[1] * k] +=
//                        a.data[a.shape.start + a.shape.strides[0] * i + a.shape.strides[1] * j] *
//                        b.data[b.shape.start + b.shape.strides[0] * j + b.shape.strides[1] * k];
//            }
//        }
//    }
//    for (size_t i = 0; i < n; i++) {
//        for (size_t j = 0; j < m; j++) {
//            for (size_t k = 0; k < p; k++) {
//                c.data[p * i + k] += a.data[m * i + j] * b.data[p * j + k];
//            }
//        }
//        std::cout << "Done " << i << " total " << t.elapsed() << "ms\n";
//    }
//    c = a * b;
    std::cout << "Took " << t.elapsed() << "ms" << "\n";
    Matrix<int> mat = {
        {1, 2, 5, 6},
        {3, 4, 7, 8}
    };
    MatrixRef<const int> matRef = mat;
//    std::cin >> mat;
    std::cout << mat;
//    Matrix<T> aa(2, 2), bb(2, 2);
//    for (auto &it : aa) {
//        it = 1;
//    }
//    for (auto &it : bb) {
//        it = 2;
//    }
//    aa += bb;
//    Matrix<T> cc = aa + bb * T(3);
//    std::cout << cc;
}