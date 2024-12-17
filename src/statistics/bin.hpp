#pragma once

#include <vector>
#include <array>
#include <algorithm>

namespace CellularAutomata::Statistics {

    template<unsigned N> std::array<float, N> bin(std::vector<float>& samples, std::array<float, N+1u>& bounds) {
        std::array<float, N> histogram{{}};

        for(float& s : samples) {
            auto it = std::find_if(bounds.begin(), bounds.end(),
                [s](float& b){ return s < b; });
            // it points to upper bound now
            unsigned binindex = (--it) - bounds.begin();
            if(binindex >= N) // binindex == 4294967295
                continue;
            histogram[binindex]++;
        }
        return histogram;
    }

};