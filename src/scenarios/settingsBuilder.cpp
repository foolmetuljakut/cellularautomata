#include "settingsBuilder.hpp"

namespace CellularAutomata::Scenarios {

SettingsBuilder& SettingsBuilder::width(size_t width) {
    _width = width;
    return *this;
}

SettingsBuilder& SettingsBuilder::height(size_t height) {
    _height = height;
    return *this;
}

SettingsBuilder& SettingsBuilder::fieldSize(float fieldSize) {
    _fieldSize = fieldSize;
    return *this;
}

SettingsBuilder& SettingsBuilder::initialCellCount(size_t cellCount) {
    _cellCount = cellCount;
    return *this;
}

SettingsBuilder& SettingsBuilder::deltaTime(float deltaTime) {
    _parametrization.deltaTime = deltaTime;
    return *this;
}

SettingsBuilder& SettingsBuilder::deltaLength(float deltaLength) {
    _parametrization.deltaLength = deltaLength;
    return *this;
}

SettingsBuilder& SettingsBuilder::movementVelocity(float movementVelocity) {
    _parametrization.movementVelocity = movementVelocity;
    return *this;
}

SettingsBuilder& SettingsBuilder::growthRate(float growthRate) {
    _parametrization.growthRate = growthRate;
    return *this;
}

SettingsBuilder& SettingsBuilder::growthMax(float growthMax) {
    _parametrization.growthMax = growthMax;
    return *this;
}

SettingsBuilder& SettingsBuilder::splitRate(float splitRate) {
    _parametrization.splitRate = splitRate;
    return *this;
}

SettingsBuilder& SettingsBuilder::splitMinSize(float splitMinSize) {
    _parametrization.splitMinSize = splitMinSize;
    return *this;
}

SettingsBuilder& SettingsBuilder::splitCurvature(float splitCurvature) {
    _parametrization.splitCurvature = splitCurvature;
    return *this;
}

SettingsBuilder& SettingsBuilder::splitRatioStandardDeviation(float splitRatioStandardDeviation) {
    _parametrization.splitRatioStandardDeviation = splitRatioStandardDeviation;
    return *this;
}

SettingsBuilder& SettingsBuilder::splitRatioMean(float splitRatioMean) {
    _parametrization.splitRatioMean = splitRatioMean;
    return *this;
}

SettingsBuilder& SettingsBuilder::initializeWith(std::function<void(sim::Cell& cell)> cellInitializer) {
    _cellInitializer = cellInitializer;
    return *this;
} 

StochasticRealisation SettingsBuilder::build() {
    StochasticRealisation r(_width, _height, _fieldSize, _cellCount);
    // refresh drived quantities based on set parameters
    _parametrization = sim::Parametrization(_parametrization);
    r.setUnits(_parametrization);
    r.initialize(_cellInitializer);
    return r;
}

sim::Parametrization SettingsBuilder::standardParametrization() {
    deltaTime(3600);// 1h in s
    deltaLength(1e-6f);// 1µm in m
    
    movementVelocity(1.3888889e-8f); // 50µm/h in m/s

    growthRate(1.504630e-5f);    // 1.3 / day ≈ 0.00001504629630 s^−1
    growthMax(9.9e-10f);        // 990 µm²

    splitRate(1.5e-5f);// 1.3 / day ≈ 0.00001504629630 s^−1
    splitMinSize(80.f); // 80 µm²
    splitCurvature(3.f); // unitfree

    splitRatioStandardDeviation(0.25f);// 2/3rds of split events occur within +/- 25% of the mothers cell size
    splitRatioMean(0.5f);// split at 50% of the mothers cell size

    // refresh drived quantities based on set parameters
    return sim::Parametrization(_parametrization);
}

SettingsBuilder& SettingsBuilder::standardSetting() {
    width(16); // fields wide
    height(16);// fields tall
    fieldSize(10);// size of an individual field in mm²
    initialCellCount(5);

    _parametrization = standardParametrization();

    initializeWith([](sim::Cell& cell) {
        cell.size = 10.f / 5.f;
        cell.pos = 128;
    });

    return *this;
}

};