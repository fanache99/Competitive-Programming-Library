#pragma once

#include <array>
#include <numeric>
#include <optional>
#include <iostream>

namespace {
    template<size_t Dim, typename T, size_t N>
    struct extract_dimension {
        std::array<T, N - 1> operator()(const std::array<T, N> &input) const;
    };

    template<typename T>
    struct extract_dimension<0, T, 2> {
        std::array<T, 1> operator()(const std::array<T, 2> &input) const;
    };

    template<typename T>
    struct extract_dimension<1, T, 2> {
        std::array<T, 1> operator()(const std::array<T, 2> &input) const;
    };

    template<typename T, typename U>
    class copy_cv {
    private:
        using t1 = std::conditional_t<std::is_const_v<T>, std::add_const_t<U>, U>;
        using t2 = std::conditional_t<std::is_volatile_v<T>, std::add_volatile_t<t1>, t1>;
    public:
        using type = t2;
    };

    template<typename T, typename U>
    using copy_cv_t = copy_cv<T, U>::type;
}

template<typename T, size_t N, bool IsRef>
class MatrixBase;

template<typename T, size_t N = 2>
using Matrix = MatrixBase<T, N, false>;

template<typename T>
using Vector = MatrixBase<T, 1, false>;

template<typename T, size_t N = 2>
using MatrixRef = MatrixBase<T, N, true>;

template<typename T, size_t N, bool IsRef>
std::istream &operator>>(std::istream &in, MatrixBase<T, N, IsRef> &m);

template<typename T, size_t N, bool IsRef>
std::ostream &operator<<(std::ostream &out, const MatrixBase<T, N, IsRef> &m);

template<typename T, size_t N, bool IsRef1, bool IsRef2>
Matrix<T, N> operator+(MatrixBase<T, N, IsRef1> m1, const MatrixBase<T, N, IsRef2> &m2);

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator+(MatrixBase<T, N, IsRef> m1, const T &value);

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator+(const T &value, MatrixBase<T, N, IsRef> m1);

template<typename T, size_t N, bool IsRef1, bool IsRef2>
Matrix<T, N> operator-(MatrixBase<T, N, IsRef1> m1, const MatrixBase<T, N, IsRef2> &m2);

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator-(MatrixBase<T, N, IsRef> m1, const T &value);

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator-(const T &value, MatrixBase<T, N, IsRef> m1);

template<typename T, bool IsRef1, bool IsRef2>
Matrix<T, 2> operator*(const MatrixBase<T, 2, IsRef1> &m1, const MatrixBase<T, 2, IsRef2> &m2);

template<typename T, bool IsRef1, bool IsRef2>
Matrix<T, 1> operator*(const MatrixBase<T, 1, IsRef1> &v, const MatrixBase<T, 2, IsRef2> &m);

template<typename T, bool IsRef1, bool IsRef2>
Matrix<T, 1> operator*(const MatrixBase<T, 2, IsRef1> &m, const MatrixBase<T, 1, IsRef2> &v);

template<typename T, bool IsRef1, bool IsRef2>
T operator*(const MatrixBase<T, 1, IsRef1> &v1, const MatrixBase<T, 1, IsRef2> &v2);

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator*(MatrixBase<T, N, IsRef> m1, const T &value);

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator*(const T &value, MatrixBase<T, N, IsRef> m1);

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator/(MatrixBase<T, N, IsRef> m1, const T &value);

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator/(const T &value, MatrixBase<T, N, IsRef> m1);

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator%(MatrixBase<T, N, IsRef> m1, const T &value);

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator%(const T &value, MatrixBase<T, N, IsRef> m1);

template<typename T, bool IsRef, typename P>
Matrix<T, 2> pow(const MatrixBase<T, 2, IsRef> &base, P power);

template<typename T, size_t N>
class MatrixInitializerImpl;

template<typename T, size_t N>
using MatrixInitializer = MatrixInitializerImpl<T, N>::type;

template<typename T, size_t N>
class MatrixInitializerImpl {
public:
    template<typename, size_t>
    friend class MatrixInitializerImpl;

    using type = std::conditional_t<(N == 1), std::initializer_list<T>, std::initializer_list<typename MatrixInitializerImpl<T, N - 1>::type>>;
    static std::array<size_t, N> extractExtents(const MatrixInitializer<T, N> &init);

private:
    template<typename It>
    static void extractExtentsImpl(It extent, const MatrixInitializer<T, N> &init);
    template<typename It>
    static void checkSameSize(It extent, const MatrixInitializer<T, N> &init);
};

template<typename T>
struct MatrixInitializerImpl<T, 0> {
    using type = void;
};

template<typename T, size_t N>
class MatrixIterator {
public:
    MatrixIterator(MatrixRef<T, N> m, std::optional<std::array<size_t, N>> point = std::nullopt);
    MatrixIterator &operator++();
    T &operator*();
    bool operator==(const MatrixIterator &other) const;
    size_t suffix();

private:
    MatrixRef<T, N> m;
    std::optional<std::array<size_t, N>> point;
    size_t offset;
};

template<size_t N>
class MatrixShape {
public:
    template<typename, size_t>
    friend
    class MatrixIterator;

    template<typename ...Extents>
    MatrixShape(Extents ...extents);
    MatrixShape(const std::array<size_t, N> &extents, const std::array<size_t, N> &strides, size_t start = 0);
    MatrixShape(const std::array<size_t, N> &extents, size_t start = 0);

    size_t extent(size_t dim) const;

    size_t size() const;

    template<typename ...Dims>
    size_t operator()(Dims ...dims) const;
    size_t operator()(const std::array<size_t, N> &point) const;
    size_t operator()(size_t i, size_t j) const;
    size_t operator()(size_t i) const;

    template<size_t Dim>
    MatrixShape<N - 1> dimension(size_t n) const;

    bool same(const MatrixShape<N> &other) const;

private:
    size_t start;
    std::array<size_t, N> extents;
    size_t _size;
    std::array<size_t, N> strides;

    size_t compute_size();

    void set_default_strides();
};

template<typename T, size_t N, bool IsRef>
class MatrixBase {
public:
    static_assert(!std::is_reference_v<T>, "Matrices cannot hold reference types.");
    static_assert(IsRef || std::is_same_v<T, std::remove_cvref_t<T>>, "Matrix types should not be cv-qualified.");
    using valueType = std::remove_cv_t<T>;

    template<typename, size_t>
    friend class MatrixIterator;

    template<typename, size_t, bool>
    friend class MatrixBase;

    static constexpr size_t dimensions = N;

    template<typename ...Extents>
    explicit MatrixBase(Extents ...extents);
    template<typename U, bool IsRefOther>
    MatrixBase(const MatrixBase<U, N, IsRefOther> &m);
    template<typename U, bool IsRefOther>
    MatrixBase(MatrixBase<U, N, IsRefOther> &m);
    MatrixBase(MatrixShape<N> shape, std::vector<valueType> &data);
    MatrixBase(MatrixShape<N> shape, const std::vector<valueType> &data);
    MatrixBase(const MatrixInitializer<T, N> &init);
    MatrixBase(const MatrixShape<N> &shape, const T &val);

    size_t size(std::optional<size_t> dim = std::nullopt) const;
    size_t rows() const;
    size_t columns() const;
    std::conditional_t<(N > 1), MatrixRef<T, N - 1>, T &> operator[](size_t n);
    std::conditional_t<(N > 1), MatrixRef<const T, N - 1>, const T &> operator[](size_t n) const;
    MatrixRef<T, N - 1> row(size_t n);
    MatrixRef<const T, N - 1> row(size_t n) const;
    MatrixRef<T, N - 1> column(size_t n);
    MatrixRef<const T, N - 1> column(size_t n) const;
    template<size_t Dim>
    MatrixRef<T, N - 1> dimension(size_t n);
    template<size_t Dim>
    MatrixRef<const T, N - 1> dimension(size_t n) const;
    template<typename ...Dims>
    requires (sizeof...(Dims) == N)
    T &operator()(Dims ...dims);
    template<typename ...Dims>
    requires (sizeof...(Dims) == N)
    const T &operator()(Dims ...dims) const;
    operator T &();

    MatrixIterator<T, N> begin();
    MatrixIterator<T, N> end();
    MatrixIterator<const T, N> begin() const;
    MatrixIterator<const T, N> end() const;

    friend std::istream &operator>><>(std::istream &in, MatrixBase<T, N, IsRef> &m);
    friend std::ostream &operator<<<>(std::ostream &out,const MatrixBase<T, N, IsRef> &m);

    template<typename Function>
    MatrixBase &apply(Function f);
    template<bool IsRef2, typename Function>
    MatrixBase &apply(const MatrixBase<T, N, IsRef2> &m2, Function f);
    MatrixBase &operator-();
    MatrixBase &operator+=(const T &value);
    MatrixBase &operator-=(const T &value);
    MatrixBase &operator/=(const T &value);
    MatrixBase &operator*=(const T &value);
    MatrixBase &operator%=(const T &value);
    template<bool IsRefOther>
    MatrixBase<T, N, IsRef> &operator+=(const MatrixBase<T, N, IsRefOther> &m);
    template<bool IsRefOther>
    MatrixBase<T, N, IsRef> &operator-=(const MatrixBase<T, N, IsRefOther> &m);
    static Matrix<T, 2> one(size_t n);

private:
    MatrixShape<N> shape;
    std::conditional_t<IsRef, copy_cv_t<T, std::vector<valueType>> &, std::vector<valueType>> data;

    static void copyFlat(std::vector<valueType> &data, const MatrixInitializer<T, N> &init);
};

/* ----------------------------------------------IMPLEMENTATION------------------------------------------------------ */
template<typename T, size_t N>
std::array<size_t, N> MatrixInitializerImpl<T, N>::extractExtents(const MatrixInitializer<T, N> &init) {
    std::array<size_t, N> extents;
    MatrixInitializerImpl<T, N>::extractExtentsImpl(extents.begin(), init);
    MatrixInitializerImpl<T, N>::checkSameSize(extents.begin(), init);
    return extents;
}

template<typename T, size_t N>
template<typename It>
void MatrixInitializerImpl<T, N>::extractExtentsImpl(It extents, const MatrixInitializer<T, N> &init) {
    assert(init.size() > 0);
    *extents = init.size();
    if constexpr (N > 1) {
        MatrixInitializerImpl<T, N - 1>::extractExtentsImpl(++extents, *init.begin());
    }
}

template<typename T, size_t N>
template<typename It>
void MatrixInitializerImpl<T, N>::checkSameSize(It extent, const MatrixInitializer<T, N> &init) {
    assert(init.size() == *extent);
    if constexpr (N > 1) {
        for (const auto &nested : init) {
            MatrixInitializerImpl<T, N - 1>::checkSameSize(extent + 1, nested);
        }
    }
}

template<typename T, size_t N>
MatrixIterator<T, N>::MatrixIterator(MatrixRef<T, N> m, std::optional<std::array<size_t, N>> point) :
        m(m), point(point), offset(m.shape(*point)) {}

template<typename T, size_t N>
MatrixIterator<T, N> &MatrixIterator<T, N>::operator++() {
    int dim;
    for (dim = N - 1; dim >= 0 && point->at(dim) + 1 == m.shape.extents[dim]; dim--) {
        offset -= m.shape.strides[dim] * point->at(dim);
        point->at(dim) = 0;
    }
    if (dim == -1) {
        point.reset();
    } else {
        point->at(dim)++;
        offset += m.shape.strides[dim];
    }
    return *this;
}

template<typename T, size_t N>
T &MatrixIterator<T, N>::operator*() {
    assert(point);
    return m.data[offset];
}

template<typename T, size_t N>
bool MatrixIterator<T, N>::operator==(const MatrixIterator &other) const {
    return point == other.point;
}

template<typename T, size_t N>
size_t MatrixIterator<T, N>::suffix() {
    assert(point);
    size_t suffix = 0;
    for (int dim = N - 1; dim >= 0 && point->at(dim) + 1 == m.shape.extents[dim]; dim--) {
        suffix++;
    }
    return suffix;
}

namespace {
    template<size_t Dim, typename T, size_t N>
    std::array<T, N - 1> extract_dimension<Dim, T, N>::operator()(const std::array<T, N> &input) const {
        std::array<T, N - 1> output;
        std::copy(input.begin(), input.begin() + Dim, output.begin());
        std::copy(input.begin() + Dim + 1, input.end(), output.begin() + Dim);
        return output;
    }

    template<typename T>
    std::array<T, 1> extract_dimension<0, T, 2>::operator()(const std::array<T, 2> &input) const {
        return {input[1]};
    }

    template<typename T>
    std::array<T, 1> extract_dimension<1, T, 2>::operator()(const std::array<T, 2> &input) const {
        return {input[0]};
    }
}

template<size_t N>
template<typename ...Extents>
MatrixShape<N>::MatrixShape(Extents ...extents) : start(0), extents{extents...}, _size(compute_size()) {
    set_default_strides();
}

template<size_t N>
MatrixShape<N>::MatrixShape(const std::array<size_t, N> &extents, const std::array<size_t, N> &strides, size_t start) :
    start(start), extents(extents), strides(strides), _size(compute_size()) {}

template<size_t N>
MatrixShape<N>::MatrixShape(const std::array<size_t, N> &extents, size_t start) :
    start(start), extents(extents), _size(compute_size()) {
    set_default_strides();
}

template<size_t N>
size_t MatrixShape<N>::extent(size_t dim) const {
    return extents[dim];
}

template<size_t N>
size_t MatrixShape<N>::size() const {
    return _size;
}

template<size_t N>
template<typename... Dims>
size_t MatrixShape<N>::operator()(Dims... dims) const {
    static_assert(sizeof...(Dims) == N);
    std::array<size_t, N> point{dims...};
    return (*this)(point);
}

template<size_t N>
size_t MatrixShape<N>::operator()(const std::array<size_t, N> &point) const {
    return start + std::inner_product(point.begin(), point.end(), strides.begin(), 0);
}

template<size_t N>
size_t MatrixShape<N>::operator()(size_t i, size_t j) const {
    static_assert(N == 2);
    return start + strides[0] * i + strides[1]  * j;
}

template<size_t N>
size_t MatrixShape<N>::operator()(size_t i) const {
    static_assert(N == 1);
    return start + strides[0] * i;
}

template<size_t N>
template<size_t Dim>
MatrixShape<N - 1> MatrixShape<N>::dimension(size_t n) const {
    return {extract_dimension<Dim, size_t, N>()(extents), extract_dimension<Dim, size_t, N>()(strides), start + strides[Dim] * n};
}

template<size_t N>
bool MatrixShape<N>::same(const MatrixShape<N> &other) const {
    return extents == other.extents;
}

template<size_t N>
void MatrixShape<N>::set_default_strides() {
    strides.back() = 1;
    std::partial_sum(extents.rbegin(), extents.rend() - 1, strides.rbegin() + 1, std::multiplies<>());
}

template<size_t N>
size_t MatrixShape<N>::compute_size() {
    return std::accumulate(extents.begin(), extents.end(), 1, std::multiplies<>());
}

template<typename T, size_t N, bool IsRef>
template<typename ...Extents>
MatrixBase<T, N, IsRef>::MatrixBase(Extents ...extents) : shape(extents...), data(this->shape.size()) {
    static_assert(!IsRef, "Cannot construct matrix reference from extents.");
}

template<typename T, size_t N, bool IsRef>
template<typename U, bool IsRefOther>
MatrixBase<T, N, IsRef>::MatrixBase(const MatrixBase<U, N, IsRefOther> &m) : shape(m.shape), data(m.data) {}

template<typename T, size_t N, bool IsRef>
template<typename U, bool IsRefOther>
MatrixBase<T, N, IsRef>::MatrixBase(MatrixBase<U, N, IsRefOther> &m) : shape(m.shape), data(m.data) {}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef>::MatrixBase(MatrixShape<N> shape, const std::vector<valueType> &data) : shape(shape), data(data) {}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef>::MatrixBase(MatrixShape<N> shape, std::vector<valueType> &data) : shape(shape), data(data) {}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef>::MatrixBase(const MatrixInitializer<T, N> &init) :
    shape(MatrixInitializerImpl<T, N>::extractExtents(init)) {
    copyFlat(data, init);
}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef>::MatrixBase(const MatrixShape<N> &shape, const T &val) : shape(shape), data(shape.size(), val) {}

template<typename T, size_t N, bool IsRef>
void MatrixBase<T, N, IsRef>::copyFlat(std::vector<valueType> &data, const MatrixInitializer<T, N> &init) {
    if constexpr (N == 1) {
        data.insert(data.end(), init.begin(), init.end());
    } else {
        for (auto &nested : init) {
            MatrixBase<T, N - 1, IsRef>::copyFlat(data, nested);
        }
    }
}

template<typename T, size_t N, bool IsRef>
size_t MatrixBase<T, N, IsRef>::size(std::optional<size_t> dim) const {
    if (dim) {
        assert(*dim >= 0 && *dim < N);
        return shape.extent(*dim);
    } else {
        return shape.size();
    }
}

template<typename T, size_t N, bool IsRef>
size_t MatrixBase<T, N, IsRef>::rows() const {
    static_assert(N == 2, "[rows] method is only available for 2-dimensional matrices.");
    return size(0);
}

template<typename T, size_t N, bool IsRef>
size_t MatrixBase<T, N, IsRef>::columns() const {
    static_assert(N == 2, "[columns] method is only available for 2-dimensional matrices.");
    return size(1);
}

template<typename T, size_t N, bool IsRef>
std::conditional_t<(N > 1), MatrixRef<T, N - 1>, T &> MatrixBase<T, N, IsRef>::operator[](size_t n) {
    static_assert(N > 0, "Cannot use subscript operator for 0-dimensional matrix.");
    if constexpr (N > 1) {
        return dimension<0>(n);
    } else {
        return (*this)(n);
    }
}

template<typename T, size_t N, bool IsRef>
std::conditional_t<(N > 1), MatrixRef<const T, N - 1>, const T &> MatrixBase<T, N, IsRef>::operator[](size_t n) const {
    static_assert(N > 0, "Cannot use subscript operator for 0-dimensional matrix.");
    if constexpr (N > 1) {
        return dimension<0>(n);
    } else {
        return (*this)(n);
    }
}

template<typename T, size_t N, bool IsRef>
MatrixRef<T, N - 1> MatrixBase<T, N, IsRef>::row(size_t n) {
    static_assert(N == 2, "[row] method is only available for 2-dimensional matrices.");
    return dimension<0>(n);
}

template<typename T, size_t N, bool IsRef>
MatrixRef<const T, N - 1> MatrixBase<T, N, IsRef>::row(size_t n) const {
    static_assert(N == 2, "[row] method is only available for 2-dimensional matrices.");
    return dimension<0>(n);
}

template<typename T, size_t N, bool IsRef>
MatrixRef<T, N - 1> MatrixBase<T, N, IsRef>::column(size_t n) {
    static_assert(N == 2, "[column] method is only available for 2-dimensional matrices.");
    return dimension<1>(n);
}

template<typename T, size_t N, bool IsRef>
MatrixRef<const T, N - 1> MatrixBase<T, N, IsRef>::column(size_t n) const {
    static_assert(N == 2, "[column] method is only available for 2-dimensional matrices.");
    return dimension<1>(n);
}

template<typename T, size_t N, bool IsRef>
template<size_t Dim>
MatrixRef<T, N - 1> MatrixBase<T, N, IsRef>::dimension(size_t n) {
    return {shape.template dimension<Dim>(n), data};
}

template<typename T, size_t N, bool IsRef>
template<size_t Dim>
MatrixRef<const T, N - 1> MatrixBase<T, N, IsRef>::dimension(size_t n) const {
    return {shape.template dimension<Dim>(n), data};
}

template<typename T, size_t N, bool IsRef>
template<typename... Dims>
requires (sizeof...(Dims) == N)
T &MatrixBase<T, N, IsRef>::operator()(Dims... dims) {
    return const_cast<T &>(static_cast<const MatrixBase<T, N, IsRef> &>(*this)(dims...));
}

template<typename T, size_t N, bool IsRef>
template<typename... Dims>
requires (sizeof...(Dims) == N)
const T &MatrixBase<T, N, IsRef>::operator()(Dims... dims) const {
    return data[shape(dims...)];
}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef>::operator T &() {
    static_assert(N == 0, "Only 0-dimensional matrices ");
    return data[0];
}

template<typename T, size_t N, bool IsRef>
MatrixIterator<T, N> MatrixBase<T, N, IsRef>::begin() {
    return MatrixIterator<T, N>(*this, std::array<size_t, N>{});
}

template<typename T, size_t N, bool IsRef>
MatrixIterator<T, N> MatrixBase<T, N, IsRef>::end() {
    return MatrixIterator<T, N>(*this);
}

template<typename T, size_t N, bool IsRef>
MatrixIterator<const T, N> MatrixBase<T, N, IsRef>::begin() const {
    return MatrixIterator<const T, N>(MatrixRef<const T, N>(*this), std::array<size_t, N>{});
}

template<typename T, size_t N, bool IsRef>
MatrixIterator<const T, N> MatrixBase<T, N, IsRef>::end() const {
    return MatrixIterator<const T, N>(*this);
}

template<typename T, size_t N, bool IsRef>
std::istream &operator>>(std::istream &in, MatrixBase<T, N, IsRef> &m) {
    for (auto &entry: m) {
        in >> entry;
    }
    return in;
}

template<typename T, size_t N, bool IsRef>
std::ostream &operator<<(std::ostream &out, const MatrixBase<T, N, IsRef> &m) {
    for (auto it = m.begin(); it != m.end(); ++it) {
        out << *it;
        int suffix = it.suffix();
        if (suffix == 0) {
            out << " ";
        } else {
            out << std::string(suffix, '\n');
        }
    }
    return out;
}

template<typename T, size_t N, bool IsRef>
template<typename Function>
MatrixBase<T, N, IsRef> &MatrixBase<T, N, IsRef>::apply(Function f) {
    for (auto &x: *this) {
        f(x);
    }
    return *this;
}

template<typename T, size_t N, bool IsRef>
template<bool IsRef2, typename Function>
MatrixBase<T, N, IsRef> &MatrixBase<T, N, IsRef>::apply(const MatrixBase<T, N, IsRef2> &m2, Function f) {
    assert(shape.same(m2.shape));
    for (auto it1 = begin(), it2 = m2.begin(); it1 != end(); ++it1, ++it2) {
        f(*it1, *it2);
    }
    return *this;
}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef> &MatrixBase<T, N, IsRef>::operator-() {
    return apply([&](T &x) { x = -x; });
}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef> &MatrixBase<T, N, IsRef>::operator+=(const T &value) {
    return apply([&](T &x) { x += value; });
}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef> &MatrixBase<T, N, IsRef>::operator-=(const T &value) {
    return apply([&](T &x) { x -= value; });
}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef> &MatrixBase<T, N, IsRef>::operator/=(const T &value) {
    return apply([&](T &x) { x /= value; });
}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef> &MatrixBase<T, N, IsRef>::operator*=(const T &value) {
    return apply([&](T &x) { x *= value; });
}

template<typename T, size_t N, bool IsRef>
MatrixBase<T, N, IsRef> &MatrixBase<T, N, IsRef>::operator%=(const T &value) {
    return apply([&](T &x) { x %= value; });
}

template<typename T, size_t N, bool IsRef>
Matrix<T, 2> MatrixBase<T, N, IsRef>::one(size_t n) {
    Matrix<T, 2> res(n, n);
    for (size_t i = 0; i < n; i++) {
        res(i, i) = 1;
    }
    return res;
}

template<typename T, size_t N, bool IsRef>
template<bool IsRefOther>
MatrixBase<T, N, IsRef> &MatrixBase<T, N, IsRef>::operator+=(const MatrixBase<T, N, IsRefOther> &m) {
    return apply(m, [&](T &x, const T &y) { x += y; });
}

template<typename T, size_t N, bool IsRef>
template<bool IsRefOther>
MatrixBase<T, N, IsRef> &MatrixBase<T, N, IsRef>::operator-=(const MatrixBase<T, N, IsRefOther> &m) {
    return apply(m, [&](T &x, const T &y) { x -= y; });
}

template<typename T, size_t N, bool IsRef1, bool IsRef2>
Matrix<T, N> operator+(MatrixBase<T, N, IsRef1> m1, const MatrixBase<T, N, IsRef2> &m2) {
    return m1 += m2;
}

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator+(MatrixBase<T, N, IsRef> m1, const T &value) {
    return m1 += value;
}

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator+(const T &value, MatrixBase<T, N, IsRef> m1) {
    return m1 += value;
}

template<typename T, size_t N, bool IsRef1, bool IsRef2>
Matrix<T, N> operator-(MatrixBase<T, N, IsRef1> m1, const MatrixBase<T, N, IsRef2> &m2) {
    return m1 -= m2;
}

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator-(MatrixBase<T, N, IsRef> m1, const T &value) {
    return m1 -= value;
}

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator-(const T &value, MatrixBase<T, N, IsRef> m1) {
    return m1.apply([&](T &x) { return value - x; });
}

template<typename T, bool IsRef1, bool IsRef2>
Matrix<T, 2> operator*(const MatrixBase<T, 2, IsRef1> &m1, const MatrixBase<T, 2, IsRef2> &m2) {
    assert(m1.columns() == m2.rows());
    size_t n = m1.rows(), m = m1.columns(), p = m2.columns();
    Matrix<T, 2> res(n, p);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            for (size_t k = 0; k < p; k++) {
                res(i, k) += m1(i, j) * m2(j, k);
            }
        }
    }
    return res;
}

template<typename T, bool IsRef1, bool IsRef2>
Vector<T> operator*(const MatrixBase<T, 1, IsRef1> &v, const MatrixBase<T, 2, IsRef2> &m) {
    assert(v.size() == m.rows());
    Vector<T> res(m.columns());
    for (size_t i = 0; i < m.rows(); i++) {
        for (size_t j = 0; j < m.columns(); j++) {
            res(j) += v(i) * m(i, j);
        }
    }
    return res;
}

template<typename T, bool IsRef1, bool IsRef2>
Vector<T> operator*(const MatrixBase<T, 2, IsRef1> &m, const MatrixBase<T, 1, IsRef2> &v) {
    assert(v.size() == m.columns());
    Vector<T> res(m.rows());
    for (size_t i = 0; i < m.rows(); i++) {
        for (size_t j = 0; j < m.columns(); j++) {
            res(i) += m(i, j) * v(j);
        }
    }
    return res;
}

template<typename T, bool IsRef1, bool IsRef2>
T operator*(const MatrixBase<T, 1, IsRef1> &v1, const MatrixBase<T, 1, IsRef2> &v2) {
    return std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);
}

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator*(MatrixBase<T, N, IsRef> m1, const T &value) {
    return m1 *= value;
}

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator*(const T &value, MatrixBase<T, N, IsRef> m1) {
    return m1 *= value;
}

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator/(MatrixBase<T, N, IsRef> m1, const T &value) {
    return m1 /= value;
}

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator/(const T &value, MatrixBase<T, N, IsRef> m1) {
    return m1.apply([&](T &x) { return value / x; });
}

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator%(MatrixBase<T, N, IsRef> m1, const T &value) {
    return m1 %= value;
}

template<typename T, size_t N, bool IsRef>
Matrix<T, N> operator%(const T &value, MatrixBase<T, N, IsRef> m1) {
    return m1.apply([&](T &x) { return value % x; });
}

template<typename T, bool IsRef, typename P>
Matrix<T, 2> pow(const MatrixBase<T, 2, IsRef> &base, P power) {
    return fastOp(base, power, Matrix<T>::one(base.rows()), std::multiplies<>());
}