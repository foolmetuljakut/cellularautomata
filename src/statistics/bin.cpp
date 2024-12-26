#include "bin.hpp"

namespace CellularAutomata::Statistics {

    std::vector<float> bin(std::vector<float>& samples, std::vector<float>& bounds) {

        size_t N = bounds.size()-1; // output length: every bin sits in between two bounds
        std::vector<float> histogram;
        histogram.resize(N);

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