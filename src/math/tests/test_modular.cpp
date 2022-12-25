#include "catch2/catch_test_macros.hpp"
#include "math/modular.h"

#include <iostream>

template<template<typename> typename ModT, typename T>
void testModuloImpl(T modulo) {
    using ST = std::make_signed_t<T>;
    using DT = doubleWidthType_T<T>;
    ModT<T> mod(modulo);
    REQUIRE(mod.modulo() == modulo);
    REQUIRE(mod.wrap(0) == 0);
    REQUIRE(mod.unwrap(mod.wrap(-1)) == modulo - 1);
    REQUIRE(mod.unwrap(mod.wrap(-1 - ST(modulo))) == modulo - 1);
    REQUIRE(mod.unwrap(mod.wrap(-ST(modulo))) == 0);
    REQUIRE(mod.unwrap(mod.wrap(1)) == 1);
    REQUIRE(mod.unwrap(mod.wrap(1000000007)) == 1000000007 % modulo);
    REQUIRE(mod.unwrap(mod.wrap(9982443530123456789ull)) == 9982443530123456789ull % modulo);
    REQUIRE(mod.unwrap(mod.wrap(modulo + 1)) == 1);
    REQUIRE(mod.unwrap(mod.increment(mod.wrap(0))) == 1);
    REQUIRE(mod.unwrap(mod.increment(mod.wrap(modulo - 1))) == 0);
    REQUIRE(mod.unwrap(mod.decrement(mod.wrap(0))) == modulo - 1);
    REQUIRE(mod.unwrap(mod.decrement(mod.wrap(modulo - 1))) == modulo - 2);
    REQUIRE(mod.unwrap(mod.add(mod.wrap(3), mod.wrap(2))) == 5 % modulo);
    REQUIRE(mod.unwrap(mod.add(mod.wrap(987654321), mod.wrap(123456789))) == (987654321 + 123456789) % modulo);
    REQUIRE(mod.unwrap(mod.subtract(mod.wrap(3), mod.wrap(2))) == 1);
    REQUIRE(mod.unwrap(mod.subtract(mod.wrap(3), mod.wrap(4))) == modulo - 1);
    REQUIRE(mod.unwrap(mod.multiply(mod.wrap(3), mod.wrap(2))) == 6 % modulo);
    REQUIRE(mod.unwrap(mod.multiply(mod.wrap(10000), mod.wrap(100000))) == DT(10000) * 100000 % modulo);
    REQUIRE(mod.unwrap(mod.multiply(mod.wrap(3), mod.wrap(-2))) == (-6 % ST(modulo) + modulo) % modulo);
    REQUIRE(mod.unwrap(mod.multiply(mod.wrap(42), mod.inverse(mod.wrap(6)))) == 7 % modulo);
    REQUIRE(mod.unwrap(mod.inverse(mod.wrap(3))) == modularInverse<T>(3, modulo));
    REQUIRE(mod.unwrap(mod.multiply(mod.wrap(3), mod.inverse(mod.wrap(6)))) == modularInverse<T>(2, modulo));
    REQUIRE(mod.unwrap(mod.multiply(mod.wrap(3), mod.inverse(mod.wrap(-6)))) == modularInverse<T>(modulo - 2, modulo));
    REQUIRE(mod.unwrap(mod.negative(mod.wrap(1))) == modulo - 1);
}

TEST_CASE("Modulo Implementations") {
    testModuloImpl<StandardModulo, uint32_t>(998244353);
    testModuloImpl<MontgomeryModulo, uint32_t>(998244353);
}

TEST_CASE("Static") {
    using Mod = VarModular<>;
    Mod a(4, 6);
    Mod b(6, 10);
    Mod c(1, 15);
    Mod d(3, 7);
    Mod res({a, b, c, d});
    std::cout << res << " " << res.modulo() << "\n";
}