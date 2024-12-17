#pragma once

#include <vector>
#include <functional>

namespace CellularAutomata::Statistics {

    template<typename Out, typename In> std::vector<Out> map(const std::vector<In>& samples, std::function<Out(const In&)> selector) {
        std::vector<Out> smpls;
        smpls.reserve(samples.size());

        for(auto& sample : samples)
            smpls.push_back(selector(sample));

        return smpls;
    }

};