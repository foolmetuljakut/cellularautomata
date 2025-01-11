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
        float calcSplitProbability(const size_t& cellIndex);

        bool cellCanMove(const size_t& cellIndex);
        bool cellCanGrow(const size_t& cellIndex);
        bool cellCanSplit(const size_t& cellIndex);
        bool determineActivity(const size_t& cellIndex);

        virtual void moveUpdate(const size_t& cellIndex);
        virtual void growthUpdate(const size_t& cellIndex);
        virtual void splitUpdate(const size_t& cellIndex);
        void update();
        StochasticRealisation& update(size_t n_cycles);
    };

};