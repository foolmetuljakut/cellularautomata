#pragma once

#include <array>

namespace CellularAutomata::Statistics {
    template<unsigned N> float min(std::array<float, N>& y) {
        float m = y[0];
        for(auto& f : y)
            if(f < m) m = f;
        return m;
    }

    template<unsigned N> float max(std::array<float, N>& y) {
        float m = y[0];
        for(auto& f : y)
            if(f > m) m = f;
        return m;
    }

};