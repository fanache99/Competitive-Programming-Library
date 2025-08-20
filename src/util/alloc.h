#pragma once

#include <vector>
#include <array>

template<size_t N>
struct StaticMemory {
    template<typename T>
    class Allocator {
    public:
        T* allocate(size_t n) {
            offset += n;
            return &data[offset - n];
        }

    private:
        static size_t offset;
        static std::array<T, N> data;
    };
};

template<size_t N>
template<typename T>
size_t StaticMemory<N>::Allocator<T>::offset{};

template<size_t N>
template<typename T>
std::array<T, N> StaticMemory<N>::Allocator<T>::data{};