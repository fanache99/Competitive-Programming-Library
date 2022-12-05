#pragma once

#include <cassert>
#include <utility>

#include "math/basic/basic.h"

template<typename ModuloPolicy, typename Parent>
class ModularBase : private ModuloPolicy {
protected:
    using T = typename ModuloPolicy::type;

public:

    template<typename ...ModArgs>
    ModularBase(T _value, ModArgs ...modArgs) : ModuloPolicy(std::forward<ModArgs>(modArgs)...), value(modImpl(_value, mod())) {}

    constexpr T mod() {
        return ModuloPolicy::mod();
    }

    constexpr bool operator == (const Parent &other) const {
        check(other);
        return value == other.value;
    }

    constexpr bool operator != (const Parent &other) const {
        return !(*this == other);
    }

    constexpr Parent& operator += (const Parent &other) {
        check(other);
        value = addImpl(value, other.value, mod());
        return static_cast<Parent&>(*this);
    }

    constexpr Parent& operator -= (const Parent &other) {
        check(other);
        value = subImpl(value, other.value, mod());
        return static_cast<Parent&>(*this);
    }

    constexpr Parent& operator *= (const Parent &other) {
        check(other);
        value = mulImpl(value, other.value, mod());
        return static_cast<Parent&>(*this);
    }

    friend constexpr Parent operator + (Parent lhs, const Parent &rhs) {
        return lhs += rhs;
    }

    friend constexpr Parent operator - (Parent lhs, const Parent &rhs) {
        return lhs -= rhs;
    }

    friend constexpr Parent operator * (Parent lhs, const Parent &rhs) {
        return lhs *= rhs;
    }

    explicit operator T() const {
        return value;
    }

protected:
    T value;

    void constexpr check(const Parent &other) {
        if constexpr (ModuloPolicy::checkModsAreSame) {
            assert(mod() == other.mod());
        }
    }

    static constexpr T modImpl(T value, T mod) {
        return (value % mod + mod) % mod;
    }

    static constexpr T addImpl(T a, T b, T mod) {
        return a + b > mod ? a + b - mod : a + b;
    }

    static constexpr T subImpl(T a, T b, T mod) {
        return a - b < 0 ? a - b + mod : a - b;
    }

    static constexpr T mulImpl(T a, T b, T mod) {
        return a * b % mod;
    }

    static constexpr bool checkMod(T mod) {
        return mod > 0 &&
               mod <= std::numeric_limits<T>::max() / 2 + 1 &&    // 2 * (MOD - 1) <= max for + operator
               mod + std::numeric_limits<T>::min() - 1 <= 0;      // -(MOD - 1) >= min for - operator
    }

    friend std::ostream& operator<<(std::ostream& out, const ModularBase& x) {
        out << x.value;
        return out;
    }
};

template<auto MOD, typename T = decltype(MOD)>
struct StaticModulo {
    using type = T;
    static constexpr bool checkModsAreSame = false;

    T mod() {
        return _mod;
    }

    static constexpr T _mod = MOD;
};

template<typename T = int>
struct DynamicModulo {
    using type = T;
    static constexpr bool checkModsAreSame = false;

    T mod() {
        return _mod;
    }

    static T _mod;
};

template<typename T>
T DynamicModulo<T>::_mod = 0;

template<typename T = int>
struct MemberModulo {
    using type = T;
    static constexpr bool checkModsAreSame = true;

    MemberModulo(T mod_) : _mod(mod_) {}

    T mod() {
        return _mod;
    }

    const T _mod;
};

template<typename ModuloPolicy = MemberModulo<>>
class Modular : public ModularBase<ModuloPolicy, Modular<ModuloPolicy>> {
    using Base = ModularBase<ModuloPolicy, Modular<ModuloPolicy>>;
public:
    using Base::Base;
};


template<auto MOD, typename T>
class Modular<StaticModulo<MOD, T>> : public ModularBase<StaticModulo<MOD, T>, Modular<StaticModulo<MOD, T>>> {
    using Base = ModularBase<StaticModulo<MOD, T>, Modular<StaticModulo<MOD, T>>>;
public:
    using Base::Base;
    static_assert(Base::checkMod(MOD), "Modulo does not pass requirements.");
};


template<typename T>
class Modular<DynamicModulo<T>> : public ModularBase<DynamicModulo<T>, Modular<DynamicModulo<T>>> {
    using Base = ModularBase<DynamicModulo<T>, Modular<DynamicModulo<T>>>;
public:
    using Base::Base;

    static constexpr void changeMod(T mod_) {
        assert(Base::checkMod(mod_));
        DynamicModulo<T>::_mod = mod_;
    }
};
