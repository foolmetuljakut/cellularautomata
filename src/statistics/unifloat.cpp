#include "unifloat.hpp"

namespace CellularAutomata::Statistics {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> rndfloat(0, 1);

    float unifloat() {
        return rndfloat(gen);
    }
};