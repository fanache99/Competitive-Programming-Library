#include "bits/stdc++.h"

class Timer {
public:
    Timer() : start(std::chrono::high_resolution_clock::now()) {}

    template<typename Precision = std::chrono::milliseconds>
    long long elapsed() {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<Precision>(now - start).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};