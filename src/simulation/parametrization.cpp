#include "parametrization.hpp"


namespace CellularAutomata::Simulation {

Parametrization::Parametrization(float deltaTime, float deltaLength, 
    float movementVelocity, 
    float growthRate,
    float growthMax,
    float splitRate,
    float splitMinSize,
    float splitCurvature,
    float splitRatioStandardDeviation,
    float splitRatioMean) : 
    deltaTime{deltaTime}, deltaLength{deltaLength},
    movementVelocity{movementVelocity}, movementProbability{movementVelocity*deltaTime/deltaLength},
    growthRate{growthRate}, growthPercentage{growthRate*deltaTime},
    growthMax{growthMax / (deltaLength*deltaLength)},
    splitRate{splitRate}, splitProbability{splitRate * deltaTime}, 
    splitMinSize{splitMinSize}, splitCurvature{splitCurvature},
    splitRatioStandardDeviation{splitRatioStandardDeviation},
    splitRatioMean{splitRatioMean}
    {
    }

    // copy constructor forwards all parameters to standard constructor
    Parametrization::Parametrization(const Parametrization& p)
        : Parametrization(p.deltaTime, p.deltaLength, p.movementVelocity, p.growthRate, p.growthMax,
        p.splitRate, p.splitMinSize, p.splitCurvature, p.splitRatioStandardDeviation, p.splitRatioMean) {}

};