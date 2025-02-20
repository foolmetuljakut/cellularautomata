#pragma once

#include <array>
#include <vector>

namespace CellularAutomata::Statistics {

    float sum(std::vector<float>& y);
    template<size_t N> float sum(std::array<float, N>& y) {
        float s = 0;
        for(unsigned i = 0; i < y.size(); i++)
            s += y[i];
        return s;
    }
    
};