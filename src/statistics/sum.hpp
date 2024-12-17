#pragma once

#include <array>

namespace CellularAutomata::Statistics {

    template<unsigned N> static float sum(std::array<float, N>& y) {
        float s = 0;
        for(unsigned i = 0; i < y.size(); i++)
            s += y[i];
        return s;
    }
    
};