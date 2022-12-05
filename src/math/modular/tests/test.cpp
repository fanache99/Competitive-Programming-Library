#include "catch2/catch_test_macros.hpp"
#include "math/modular/modular.h"

#include <iostream>

TEST_CASE("Static") {
    Modular<StaticModulo<1000000007, long long>> x(3);
    static_assert(sizeof(x) == sizeof(long long));
    Modular<MemberModulo<>> y(3, 7);
    static_assert(sizeof(y) == 2 * sizeof(int));
    Modular<DynamicModulo<>>::changeMod(2);
    Modular<DynamicModulo<>> z(3);
    static_assert(sizeof(z) == sizeof(int));
    std::cout << x << "\n";
    x += x;
    std::cout << x << "\n";
    x += 3;
    std::cout << x << "\n";
    x = 3 + 3;
    std::cout << x << "\n";
    x = 2;
    x = (x += x);
    x = 2;
    x = pow(x, 1000000007 - 2) * 2;
    std::cout << x << "\n";
}