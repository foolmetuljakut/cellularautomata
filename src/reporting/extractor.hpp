#pragma once

#include "../std.hpp"
#include "../simulation.hpp"
namespace sim = CellularAutomata::Simulation;

namespace CellularAutomata::Reporting {

    typedef std::function<float(const sim::Cell&)> IndividualCellExtractor;
    typedef std::function<float(const sim::Field&)> IndividualFieldExtractor;
    typedef std::function<float(const float&)> IndividualPostProcessor;

    typedef std::function<float(const std::vector<sim::Cell>&)> CollectionCellStatMap;

    typedef std::function<std::vector<float>(const std::vector<sim::Cell>&)> CollectionCellExtractor;
    typedef std::function<std::vector<float>(const std::vector<sim::Field>&)> CollectionFieldExtractor;
    typedef std::function<std::vector<float>(const std::vector<float>&)> CollectionPostProcessor;
    
};