#pragma once

#include "../std.hpp"

#include "../statistics.hpp"
namespace stats = CellularAutomata::Statistics;

#include "../simulation.hpp"
namespace sim = CellularAutomata::Simulation;

namespace CellularAutomata::Scenarios {

    class StochasticRealisation : public sim::Realisation {

    public:
        std::array<float, 8> xDiff, yDiff;

        StochasticRealisation(size_t width, size_t height, float fieldSize, size_t numberOfCells);

        size_t neighbouringField(size_t fieldIndex, float r);

        virtual void moveUpdate(const size_t& cellIndex);
        virtual void growthUpdate(const size_t& cellIndex);
        virtual void splitUpdate(const size_t& cellIndex);
        void update();
    };

};