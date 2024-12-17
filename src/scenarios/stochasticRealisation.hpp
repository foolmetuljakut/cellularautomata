#pragma once

#include "../std.hpp"

#include "../statistics.hpp"
namespace stats = CellularAutomata::Statistics;

#include "../simulation.hpp"
namespace sim = CellularAutomata::Simulation;

namespace CellularAutomata::Scenarios {

    class IStochasticRealisation : public sim::Realisation {

    public:
        std::array<float, 8> xDiff, yDiff;

        IStochasticRealisation(size_t width, size_t height, float fieldSize, size_t numberOfCells);

        size_t neighbouringField(size_t fieldIndex, float r);

        virtual void moveUpdate(const size_t& cellIndex) = 0; /*mocked in tests*/
        virtual void growthUpdate(const size_t& cellIndex) = 0;/*mocked in tests*/
        virtual void splitUpdate(const size_t& cellIndex) = 0;/*mocked in tests*/
        void update();
    };

    class StochasticRealisation: public IStochasticRealisation {
    
    public:
        StochasticRealisation(size_t width, size_t height, float fieldSize, size_t numberOfCells);

        void moveUpdate(const size_t& cellIndex);
        void growthUpdate(const size_t& cellIndex);
        void splitUpdate(const size_t& cellIndex);
    };

};