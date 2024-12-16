#pragma once

#include <array>

template<unsigned N> float mean(const std::array<float, N>& y) {
    float m = 0;
    for(auto& f : y)
        m += f;
    return m / y.size();
}