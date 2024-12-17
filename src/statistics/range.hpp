#pragma once

#include <vector>

namespace CellularAutomata::Statistics {

    std::vector<size_t> range(unsigned N);
    std::vector<float> range(unsigned N, float start, float stop);

};