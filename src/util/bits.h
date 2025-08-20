#pragma once

template<typename T>
bool isPower2(T n) {
    return !(n & (n - 1));
}