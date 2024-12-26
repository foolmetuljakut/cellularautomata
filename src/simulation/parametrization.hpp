#pragma once

#include "../std.hpp"

#include "../statistics.hpp"
namespace stats = CellularAutomata::Statistics;

namespace CellularAutomata::Simulation {

    class Parametrization {
    
    public:
        float deltaTime, deltaLength;
        float movementVelocity, movementProbability;// m/s -> unitfree [0,1]
        float growthRate, growthPercentage,         // µm²/d -> unitfree [0,1]
            growthMax;                              // µm² -> unitfree [0,1]
        float splitRate, splitProbability,          // 1/d -> unitfree [0,1]
            splitMinSize,                           // µm² -> unitfree [0,1]
            splitCurvature,                         // unitfree [0,1]
            splitRatioStandardDeviation,            // unitfree [0,1]
            splitRatioMean;                         // unitfree [0,1]
        
        Parametrization() {}
        Parametrization(float deltaTime, float deltaLength, 
                        float movementVelocity, 
                        float growthRate,
                        float growthMax,
                        float splitRate,
                        float splitMinSize,
                        float splitCurvature,
                        float splitRatioStandardDeviation,
                        float splitRatioMean);
        Parametrization(const Parametrization& p);
    };

};