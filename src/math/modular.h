#pragma once

#include "bits/stdc++.h"

#include "math/basic.h"
#include "util/typeTraits.h"

template<typename T>
T modularInverse(T x, T mod) {
    static_assert(std::is_unsigned_v<T>, "T should be unsigned for modulo classes.");
    auto [ans, _, d] = euclid<std::make_signed_t<T>>(x, mod);
    assert(d == 1);
    return ans < 0 ? ans + mod : ans;
}

// Standard modulo implementation using the builtin "%" operator.
template<typename T = uint32_t>
class StandardModulo {
    static_assert(std::is_unsigned_v<T>, "T should be unsigned for modulo classes.");
    using signedT = std::make_signed_t<T>;
    using doubleWidthT = doubleWidthType_T<T>;

public:
    constexpr explicit StandardModulo(T _mod) :
        mod((checkMod(_mod), _mod)) {}

    template<typename U, std::enable_if_t<std::is_integral_v<U> && std::is_signed_v<U>>* = nullptr>
    constexpr T wrap(U x) const {
        signedT y = x % signedT(mod);
        return y < 0 ? y + mod : y;
    }

    template<typename U, std::enable_if_t<std::is_integral_v<U> && std::is_unsigned_v<U>>* = nullptr>
    constexpr T wrap(U x) const {
        return x % mod;
    }

    constexpr T unwrap(T x) const {
        return x;
    }

    constexpr T increment(T x) const {
        return shrink(x + 1);
    }

    constexpr T add(T x, T y) const {
        return shrink(x + y);
    }

    constexpr T decrement(T x) const {
        return shrink(x + mod - 1);
    }

    constexpr T subtract(T x, T y) const {
        return shrink(x + mod - y);
    }

    constexpr T negative(T x) const {
        return x == 0 ? 0 : mod - x;
    }

    constexpr T inverse(T x) const {
        return modularInverse(x, mod);
    }

    constexpr T multiply(T x, T y) const {
        return doubleWidthT(x) * y % mod;
    }

    T modulo() const {
        return mod;
    }

private:
    static constexpr void checkMod(T mod) {
        assert(mod <= std::numeric_limits<T>::max() / 2);
    }

    constexpr T shrink(T x) const {
        return x >= mod ? x - mod : x;
    }

    T mod;
};

// Montgomery reduction with R = 2 ^ (bit-width of T).
template<typename T = uint32_t>
class MontgomeryModulo {
    static_assert(std::is_unsigned_v<T>, "T should be unsigned for modulo classes.");
    using signedT = std::make_signed_t<T>;
    using doubleWidthT = doubleWidthType_T<T>;
    static constexpr int width = std::numeric_limits<T>::digits;

public:
    template<typename U>
    constexpr explicit MontgomeryModulo(U mod) :
        mod((checkMod(mod), mod)),
        modNegInv(invModR(~(mod - 1))),
        R((doubleWidthT(1) << width) % mod),
        R2(doubleWidthT(R) * R % mod),
        R3(doubleWidthT(R2) * R % mod) {}

    template<typename U, std::enable_if_t<std::is_integral_v<U> && std::is_signed_v<U>>* = nullptr>
    constexpr T wrap(U x) const {
        signedT y = x % signedT(mod);
        return redc(R2 * doubleWidthT(y < 0 ? y + mod : y));
    }

    template<typename U, std::enable_if_t<std::is_integral_v<U> && std::is_unsigned_v<U>>* = nullptr>
    constexpr T wrap(U x) const {
        return redc(R2 * doubleWidthT(x % mod));
    }

    constexpr T unwrap(T x) const {
        T ans = redc(x);
        return ans >= mod ? ans - mod : ans;
    }

    constexpr T increment(T x) const {
        return shrink(x + R);
    }

    constexpr T add(T x, T y) const {
        return shrink(x + y);
    }

    constexpr T decrement(T x) const {
        return shrink(x + mod - R);
    }

    constexpr T subtract(T x, T y) const {
        return shrink(x + 2 * mod - y);
    }

    constexpr T negative(T x) const {
        return x == 0 ? 0 : 2 * mod - x;
    }

    constexpr T multiply(T x, T y) const {
        return redc(doubleWidthT(x) * y);
    }

    constexpr T inverse(T x) const {
        return redc(doubleWidthT(R3) * modularInverse(x, mod));
    }

    T modulo() const {
        return mod;
    }

private:
    static constexpr void checkMod(T mod) {
        assert((mod & 1) == 1 && mod <= std::numeric_limits<T>::max() / 4);
    }

    // Returns y such that x * y = 1 (mod R). Uses Hensel lifting.
    static constexpr T invModR(T x) {
        T y = 1;
        for (int i = 1; i < width; i *= 2) {
            y *= T(2) - x * y;
        }
        return y;
    }

    constexpr T redc(doubleWidthT x) const {
        T m = T(x) * modNegInv;
        T t = (x + doubleWidthT(m) * mod) >> width;
        return t;
    }

    constexpr T shrink(T x) const {
        return x >= 2 * mod ? x - 2 * mod : x;
    }

    T mod;
    T modNegInv;    // mod * modNegInv = -1 (mod R)
    T R;            // R % mod
    T R2;           // R^2 % mod
    T R3;           // R^3 % mod
};

template<typename T = uint32_t>
class ExtendedMontgomeryModulo {
    static_assert(std::is_unsigned_v<T>, "T should be unsigned for modulo classes.");
    using signedT = std::make_signed_t<T>;
    using doubleWidthT = doubleWidthType_T<T>;

public:
    MontgomeryModulo<T> odd;
    T even;
};

template<typename ModuloPolicy>
class Modular : public ModuloPolicy {
public:
    using T = typename ModuloPolicy::type;
    using ModT = typename ModuloPolicy::modType;
    using Self = Modular<ModuloPolicy>;

    constexpr Modular() : value(mod().wrap(0)) {}

    template<typename U, std::enable_if_t<std::is_integral_v<U>>* = nullptr>
    constexpr Modular(U value) : value(mod().wrap(value)) {}

    constexpr T modulo() {
        return mod().modulo();
    }

    constexpr Self& operator++ () {
        value = mod().increment(value);
        return *this;
    }

    constexpr Self operator++(int) {
        Self ans = *this;
        ++(*this);
        return ans;
    }

    constexpr Self operator+() {
        return *this;
    }

    constexpr Self& operator--() {
        value = mod().decrement(value);
        *this;
    }

    constexpr Self operator--(int) {
        Self ans = *this;
        --(*this);
        return ans;
    }

    constexpr Self operator-() {
        Self ans = *this;
        ans.value = mod().negate(ans.value);
        return ans;
    }

    constexpr Self& operator += (const Self &other) {
        check(*this, other);
        value = mod().add(value, other.value);
        return *this;
    }

    constexpr Self& operator -= (const Self &other) {
        check(*this, other);
        value = mod().subtract(value, other.value);
        return *this;
    }

    constexpr Self& operator *= (const Self &other) {
        check(*this, other);
        value = mod().multiply(value, other.value);
        return *this;
    }

    constexpr Self& operator /= (const Self &other) {
        check(*this, other);
        value = mod().multiply(value, mod().inverse(other.value));
        return *this;
    }

    friend constexpr bool operator == (const Self &x1, const Self &x2) {
        check(x1, x2);
        return x1.mod().unwrap(x1.value) == x2.mod().unwrap(x2.value);
    }

    friend constexpr bool operator != (const Self &x1, const Self &x2) {
        return !(x1 == x2);
    }

    friend constexpr Self operator + (Self lhs, const Self &rhs) {
        return lhs += rhs;
    }

    friend constexpr Self operator - (Self lhs, const Self &rhs) {
        return lhs -= rhs;
    }

    friend constexpr Self operator * (Self lhs, const Self &rhs) {
        return lhs *= rhs;
    }

    friend constexpr Self operator / (Self lhs, const Self &rhs) {
        return lhs /= rhs;
    }

    explicit operator T() const {
        return mod().unwrap(value);
    }

    friend std::ostream& operator<<(std::ostream& out, const Self& x) {
        out << T(x);
        return out;
    }

    constexpr const ModT& mod() const {
        return ModuloPolicy::mod();
    }

protected:
    T value;

    static constexpr void check(const Self &x1, const Self &x2) {
        if constexpr (ModuloPolicy::checkModsAreSame) {
            assert(x1.modulo() == x2.modulo());
        }
    }

    // Used for lazy initialization of [value] by derived classes (due to order of base and member construction).
    constexpr Modular(std::nullptr_t) : value(0) {}
};

template<auto MOD, typename T = std::make_unsigned_t<decltype(MOD)>, template<typename> typename ModT = StandardModulo>
struct StaticModuloPolicy {
    using type = T;
    using modType = ModT<T>;
    static constexpr bool checkModsAreSame = false;

    static constexpr const modType& mod() {
        return _mod;
    }

    static constexpr modType _mod{MOD};
};

template<typename T = unsigned int, template<typename> typename ModT = StandardModulo>
struct DynamicModuloPolicy {
    using type = T;
    using modType = ModT<T>;
    static constexpr bool checkModsAreSame = false;

    static constexpr const modType& mod() {
        return _mods.back();
    }

    class Context {
    public:
        Context(const Context&) = delete;
        Context(Context&&) = delete;
        Context& operator=(const Context&) = delete;
        Context& operator=(Context&&) = delete;
        ~Context() {
            DynamicModuloPolicy<T, ModT>::_mods.pop_back();
        }

    private:
        friend DynamicModuloPolicy<T, ModT>;
        Context() = default;
    };

    static constexpr Context changeMod(T mod) {
        DynamicModuloPolicy<T, ModT>::_mods.emplace_back(mod);
        return Context();
    }

    static inline std::vector<modType> _mods;
};

template<typename ModuloPolicy = DynamicModuloPolicy<>>
class VarModular : public Modular<ModuloPolicy> {
    using Base = typename VarModular::Modular;
    using T = typename Base::T;
    using ModT = typename Base::ModT;

public:
    using Base::Base;
    struct NoSolution : public std::exception {};

    template<typename U, std::enable_if_t<std::is_integral_v<U>>* = nullptr>
    explicit constexpr VarModular(U _value, typename Base::T modulo) : Base(nullptr), _mod(modulo) {
        this->value = mod().wrap(_value);
    }

    // Use Chinese Remainder Theorem to combine multiple Modulo instances into one.
    explicit constexpr VarModular(const std::vector<VarModular> &others) : VarModular(0, 1) {
        for (const auto &other : others) {
            *this = combine(*this, other);
        }
    }

    explicit constexpr VarModular(const Modular<ModuloPolicy> &base) : Base(base), _mod(base.mod()) {}

    template<typename OtherModuloPolicy>
    explicit constexpr VarModular(const Modular<OtherModuloPolicy> &base) : Base(nullptr), _mod(base.mod()) {
        this->value = mod().wrap(typename Modular<OtherModuloPolicy>::T(base));
    }

    // Redefine this so that the right [mod] method is called.
    constexpr T modulo() {
        return mod().modulo();
    }

    constexpr const ModT& mod() const {
        return _mod;
    }

private:
    static constexpr VarModular combine(VarModular a, VarModular b) {
        using SignedT = std::make_signed_t<T>;
        SignedT aValue = T(a), bValue = T(b);
        auto [x, y, d] = euclid<SignedT>(a.modulo(), b.modulo());
        if ((aValue - bValue) % d != 0) {
            throw NoSolution();
        }
        T modulo = a.modulo() / d * b.modulo();
        ModT mod(modulo);
        T value = mod.unwrap(mod.add(
                mod.multiply(mod.multiply(mod.wrap(aValue), mod.wrap(y)), mod.wrap(b.modulo() / d)),
                mod.multiply(mod.multiply(mod.wrap(bValue), mod.wrap(x)), mod.wrap(a.modulo() / d))));
        return VarModular(value, modulo);
    }

    ModT _mod;
};

