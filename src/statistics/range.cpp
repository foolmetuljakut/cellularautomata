#include "range.hpp"

namespace CellularAutomata::Statistics {

    std::vector<size_t> range(unsigned N) {
        std::vector<size_t> r;
        for(size_t i = 0; i < N; i++)
            r.push_back(i);
        return r;
    }

    std::vector<float> range(unsigned N, float start, float stop) {
        std::vector<float> r;
        float step = (stop - start) / (N-1);
        for(size_t i = 0; i < N; i++)
            r.push_back(start + i * step);
        return r;
    }

};