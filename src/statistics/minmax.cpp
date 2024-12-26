#include "minmax.hpp"


namespace CellularAutomata::Statistics {
    
    float min(std::vector<float>& y) {
        float m = y[0];
        for(auto& f : y)
            if(f < m) m = f;
        return m;
    }

    float max(std::vector<float>& y) {
        float m = y[0];
        for(auto& f : y)
            if(f > m) m = f;
        return m;
    }

};