#include "sum.hpp"

namespace CellularAutomata::Statistics {

    float sum(std::vector<float>& y) {
        float s = 0;
        for(unsigned i = 0; i < y.size(); i++)
            s += y[i];
        return s;
    }
    
};