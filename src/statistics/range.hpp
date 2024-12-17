#pragma once

#include <array>

namespace CellularAutomata::Statistics {

    template<unsigned N> static std::array<float, N> range() {
        std::array<float, N> r;
        for(unsigned i = 0; i < N; i++)
            r[i] = i;
        return r;
    }

    template<unsigned N> static std::array<float, N> range(float start, float stop) {
        std::array<float, N> r;
        float step = (stop - start) / (N-1);
        for(unsigned i = 0; i < N; i++)
            r[i] = start + i * step;
        return r;
    }

};