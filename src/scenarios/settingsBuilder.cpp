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

SettingsBuilder& SettingsBuilder::splitRate(float splitRate) {
    _parametrization.splitRate = splitRate;
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
    r.setUnits(_parametrization);
    r.initialize(_cellInitializer);
    return r;
}

SettingsBuilder& SettingsBuilder::standardSetting() {
    width(16); // fields wide
    height(16);// fields tall
    fieldSize(10);// size of an individual field in mm²
    initialCellCount(5);

    deltaTime(1);// 1s
    deltaLength(0.001f);// 1mm
    movementVelocity(0.001f);// 1mm/s
    growthRate(1.1574e-11f);// (1 millimeter²) / day ≈ 1.157407407E−11 m²/s
    splitRate(1.1574e-5f);// 1 / day ≈ 0.00001157407407 s^−1
    splitRatioStandardDeviation(0.25f);// 2/3rds of split events occur within +/- 25% of the mean
    splitRatioMean(0.5f);// split at 50% of the mothers cell size

    initializeWith([](sim::Cell& cell) {
        cell.size = 10.f / 5.f;
        cell.pos = 128;
    });

    return *this;
}

};