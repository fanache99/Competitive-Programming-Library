#pragma once

template<typename Type>
struct nextSizeType {};

template<>
struct nextSizeType<int> {
    using type = long long;
};

template<>
struct nextSizeType<long long> {
    using type = __int128;
};

template<>
struct nextSizeType<unsigned int> {
    using type = unsigned long long;
};

template<>
struct nextSizeType<unsigned long long> {
    using type = unsigned __int128;
};

template<typename T>
using nextSizeType_t = typename nextSizeType<T>::type;


template<template<typename...> typename T, template<typename...> typename U>
struct isSameTemplate : std::false_type {};

template<template<typename...> typename T>
struct isSameTemplate<T, T> : std::true_type {};

template<typename T, typename... Ts>
constexpr bool typeInList()
{ return std::disjunction_v<std::is_same<T, Ts>...>; }

template<template<typename...> typename T, template<typename...> typename... Ts>
constexpr bool typeInList()
{ return std::disjunction_v<isSameTemplate<T, Ts>...>; }
