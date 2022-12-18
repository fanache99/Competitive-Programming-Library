#include "catch2/catch_test_macros.hpp"
#include "math/modular.h"

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

    using Mod = Modular<>;
    Mod a(4, 6);
    Mod b(6, 10);
    Mod c(1, 15);
    Mod d(3, 7);
    Mod res({a, b, c, d});
    std::cout << res << " " << res.mod() << "\n";
}