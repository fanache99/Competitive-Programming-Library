#pragma once

#include "bits/stdc++.h"

#include "math/basic.h"
#include "util/typeTraits.h"

template<typename ModuloPolicy, typename Parent>
class ModularBase : protected ModuloPolicy {
protected:
    using T = typename ModuloPolicy::type;
    using bigT = nextSizeType_t<T>;

public:

    template<typename ...ModArgs>
    constexpr ModularBase(T _value = 0, ModArgs ...modArgs) : ModuloPolicy(std::forward<ModArgs>(modArgs)...), value(modImpl(_value, mod())) {}

    constexpr T mod() const {
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

    constexpr Parent& operator /= (const Parent &other) {
        check(other);
        value = mulImpl(value, inverse(other.value, mod()), mod());
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

    friend constexpr Parent operator / (Parent lhs, const Parent &rhs) {
        return lhs /= rhs;
    }

    explicit operator T() const {
        return value;
    }

protected:
    T value;

    void constexpr check(const Parent &other) const {
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
        return a < b ? mod + a - b : a - b;
    }

    static constexpr T mulImpl(T a, T b, T mod) {
        return static_cast<bigT>(a) * static_cast<bigT>(b) % static_cast<bigT>(mod);
    }

    static constexpr T inverse(T a, T mod) {
        auto [x, _, d] = euclid(a, mod);
        assert(d == 1);
        return repairUnsigned(x, mod);
    }

    static constexpr bool checkMod(T mod) {
        return mod > 0 && mod <= std::numeric_limits<T>::max() / 2;    // 2 * MOD <= max for +/- operators
    }

    friend std::ostream& operator<<(std::ostream& out, const ModularBase& x) {
        out << x.value;
        return out;
    }

    T static constexpr repairUnsigned(T x, T mod) {
        if constexpr (std::is_signed_v<T>) {
            return x >= 0 && x < mod ? x : x + mod;
        } else {
            return x;
        }
    }
};

template<auto MOD, typename T = decltype(MOD)>
struct StaticModulo {
    using type = T;
    static constexpr bool checkModsAreSame = false;

    constexpr T mod() const {
        return _mod;
    }

    static constexpr T _mod = MOD;
};

template<typename T = int>
struct DynamicModulo {
    using type = T;
    static constexpr bool checkModsAreSame = false;

    constexpr T mod() const {
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

    constexpr MemberModulo(T mod_) : _mod(mod_) {}

    constexpr T mod() const {
        return _mod;
    }

    T _mod;
};

template<typename ModuloPolicy = MemberModulo<>>
class Modular : public ModularBase<ModuloPolicy, Modular<ModuloPolicy>> {
    using Base = ModularBase<ModuloPolicy, Modular<ModuloPolicy>>;
public:
    operator Modular<MemberModulo<typename ModuloPolicy::type>> () {
        return Modular<MemberModulo<typename Base::T>>(this->value, Base::mod());
    }
    using Base::Base;
};


template<auto MOD, typename T>
class Modular<StaticModulo<MOD, T>> : public ModularBase<StaticModulo<MOD, T>, Modular<StaticModulo<MOD, T>>> {
    using Base = ModularBase<StaticModulo<MOD, T>, Modular<StaticModulo<MOD, T>>>;

public:
    using Base::Base;
    static_assert(Base::checkMod(MOD), "Modulo does not pass requirements.");

    operator Modular<MemberModulo<typename Base::type>> () {
        return Modular<MemberModulo<typename Base::T>>(this->value, Base::mod());
    }
};


template<typename T>
class Modular<DynamicModulo<T>> : public ModularBase<DynamicModulo<T>, Modular<DynamicModulo<T>>> {
    using Base = ModularBase<DynamicModulo<T>, Modular<DynamicModulo<T>>>;

public:
    using Base::Base;
    operator Modular<MemberModulo<typename Base::type>> () {
        return Modular<MemberModulo<typename Base::T>>(this->value, Base::mod());
    }

    static constexpr void changeMod(T mod_) {
        assert(Base::checkMod(mod_));
        DynamicModulo<T>::_mod = mod_;
    }
};


template<typename T>
class Modular<MemberModulo<T>> : public ModularBase<MemberModulo<T>, Modular<MemberModulo<T>>> {
    using Base = ModularBase<MemberModulo<T>, Modular<MemberModulo<T>>>;

public:
    using Base::Base;
    struct NoSolution : public std::exception {};

    // Use Chinese Remainder Theorem to combine multiple Modulo instances into one.
    explicit constexpr Modular(const std::vector<Modular> &others) : Modular(0, 1) {
        for (const auto &other : others) {
            *this = combine(*this, other);
        }
    }

private:
    static constexpr Modular combine(Modular a, Modular b) {
        auto [x, y, d] = euclid(a.mod(), b.mod());
        if ((a.value - b.value) % d != 0) {
            throw NoSolution();
        }
        T mod = a.mod() / d * b.mod();
        x = Base::repairUnsigned(x, mod), y = Base::repairUnsigned(y, mod);
        return Modular(Base::addImpl(
                Base::mulImpl(Base::mulImpl(a.value, y, mod), b.mod() / d, mod),
                Base::mulImpl(Base::mulImpl(b.value, x, mod), a.mod() / d, mod),
                mod), mod);
    }
};


template<typename T, typename P>
constexpr Modular<MemberModulo<T>> pow(Modular<MemberModulo<T>> base, P power) {
    return fastOp(base, power, Modular<MemberModulo<T>>(1, base.mod()), [](auto a, auto b) { return a * b; });
}

