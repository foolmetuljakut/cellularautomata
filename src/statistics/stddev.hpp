#pragma once

#include <array>
#include <cmath>

template<unsigned N> float stddev(std::array<float, N>& y, float mean) {
    float f2 = 0;
    for(auto& f : y)
        f2 += f*f;
    return sqrt((f2 - mean*mean) / (y.size()-1));
}