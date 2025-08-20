
#include "catch2/catch_test_macros.hpp"
#include <iostream>
#include "math/matrix.h"
#include "util/timer.h"
#include "math/modular.h"

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
    Matrix<int, 2> a = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };
    Vector<int> c = a.column(1);
    std::cout << c;
//    std::cout << c.data.size();
    std::cout.flush();
}