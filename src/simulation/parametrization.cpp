#include "parametrization.hpp"


namespace CellularAutomata::Simulation {

Parametrization::Parametrization(float deltaTime, float deltaLength, 
    float movementVelocity, 
    float growthRate,
    float splitRate,
    float splitRatioStandardDeviation,
    float splitRatioMean) : 
    deltaTime{deltaTime}, deltaLength{deltaLength},
    movementVelocity{movementVelocity}, movementProbability{movementVelocity*deltaTime/deltaLength},
    growthRate{growthRate}, growthPercentage{growthRate/(deltaLength*deltaLength)*deltaTime},
    splitRate{splitRate}, splitProbability{splitRate * deltaTime},
    splitRatioStandardDeviation{splitRatioStandardDeviation},
    splitRatioMean{splitRatioMean}
    {
        spdlog::info("per-step parameters:");
        spdlog::info("p(move) = {0}", movementProbability);
        spdlog::info("c(grow) = {0}", growthPercentage);
        spdlog::info("p(split) = {0}", splitProbability);
    }

};