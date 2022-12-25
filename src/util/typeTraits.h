#pragma once

template<typename Type>
struct doubleWidthType {};

template<>
struct doubleWidthType<unsigned char> {
    using type = unsigned short int;
};

template<>
struct doubleWidthType<unsigned short int> {
    using type = unsigned int;
};

template<>
struct doubleWidthType<unsigned int> {
    using type = unsigned long long;
};

template<>
struct doubleWidthType<unsigned long long> {
    using type = unsigned __int128;
};

template<typename T>
using doubleWidthType_T = typename doubleWidthType<T>::type;


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
