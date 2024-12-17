#pragma once

#include "../std.hpp"

#include "../simulation.hpp"
namespace sim = CellularAutomata::Simulation;

#include "../scenarios/stochasticRealisation.hpp"

namespace CellularAutomata::Scenarios {

    class SettingsBuilder {
        size_t _width, _height, _cellCount;
        float _fieldSize;
        sim::Parametrization _parametrization;
        std::function<void(sim::Cell& cell)> _cellInitializer;

    public:
        StochasticRealisation build(); // returns a readily configured stochastic realisation
        sim::Parametrization standardParametrization();
        SettingsBuilder& standardSetting(); // the builder with standard values

        SettingsBuilder& width(size_t width);
        SettingsBuilder& height(size_t height);
        SettingsBuilder& fieldSize(float fieldSize);
        SettingsBuilder& initialCellCount(size_t cellCount);
        SettingsBuilder& deltaTime(float deltaTime);
        SettingsBuilder& deltaLength(float deltaLength);
        SettingsBuilder& movementVelocity(float movementVelocity);
        SettingsBuilder& growthRate(float growthRate);
        SettingsBuilder& growthMax(float growthMax);
        SettingsBuilder& splitRate(float splitRate);
        SettingsBuilder& splitMinSize(float splitMinSize);
        SettingsBuilder& splitCurvature(float splitCurvature);
        SettingsBuilder& splitRatioStandardDeviation(float splitRatioStandardDeviation);
        SettingsBuilder& splitRatioMean(float splitRatioMean);
        SettingsBuilder& initializeWith(std::function<void(sim::Cell& cell)> cellInitializer);
    };
};