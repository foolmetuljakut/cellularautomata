#pragma once

#include "../std.hpp"

#include <sciplot/sciplot.hpp>

namespace CellularAutomata::Reporting {

    class DistributionPlot {
        std::string filename;
    public:
        DistributionPlot(std::string filename);
    };
};