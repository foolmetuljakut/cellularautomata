#include "stochasticRealisation.hpp"

namespace CellularAutomata::Scenarios {

IStochasticRealisation::IStochasticRealisation(size_t width, size_t height, float fieldSize, size_t numberOfCells)
    : Realisation(width, height, fieldSize, numberOfCells), 
    xDiff{-1, -1, -1,  0, 0,  1, 1, 1},
    yDiff{-1,  0,  1, -1, 1, -1, 0, 1} {

    }

size_t IStochasticRealisation::neighbouringField(size_t fieldIndex, float r) {
    auto& field = fields[fieldIndex];
    float x = field.x, y = field.y;

    size_t randomIndex = static_cast<size_t>(8 * r);
    if(randomIndex == 8) { randomIndex--; } // vanishingly small probability, but needs to be treated

    x += xDiff[randomIndex];
    y += yDiff[randomIndex];

    if(x >= 0 && y >= 0 && x < width && y < height) {
        /*
            Explanation: ctor defines field init as: 
                for(size_t i = 0; i < width; i++) {
                    for(size_t j = 0; j < height; j++) { ... }
                }
            i.e., for i = x and j = y we increment x for each entire sequence (=height) of y
         */
        return x * height + y;
    }
    return sim::invalidIndex;
}

void IStochasticRealisation::update() {
    if(!unitProxySet) {
        throw "unit system has to be defined. set unit proxy.";
    }

    std::vector<size_t> cellIndexSequence = stats::range(cells.size()),
                        actionIndexSequence = stats::range(3);

    std::random_device rd;
    std::mt19937 gen(rd());
    if(cellIndexSequence.size() > 1) {
        std::shuffle(cellIndexSequence.begin(), cellIndexSequence.end(), gen);
    }
    std::shuffle(actionIndexSequence.begin(), actionIndexSequence.end(), gen);

    spdlog::info("performing update step");
    // TODO track down bug: no update calls (growth should be happening always)
    std::uniform_int_distribution uniInt(0, 2);
    for(auto cellIndex : cellIndexSequence) {
        for(size_t actionIndex : actionIndexSequence) {
            switch(actionIndex) {
                case 0:
                    moveUpdate(cellIndex);
                    break;
                case 1:
                    growthUpdate(cellIndex);
                    break;
                case 2:
                    splitUpdate(cellIndex);
                    break;
                default:
                throw "IStochasticRealisation::update(): only three actions (0, 1, 2) supported.";
            }
        }
    }
    spdlog::info("----------------------");
}

StochasticRealisation::StochasticRealisation(size_t width, size_t height, float fieldSize, size_t numberOfCells)
    : IStochasticRealisation(width, height, fieldSize, numberOfCells) {

}

void StochasticRealisation::moveUpdate(const size_t& cellIndex) {
    // divide by 3 as all three actions are called in one update
    bool moveTrigger{stats::unifloat() < params.movementProbability / 3.f};
    if(moveTrigger) {
        size_t newFieldIndex = neighbouringField(cells[cellIndex].pos, stats::unifloat());
        spdlog::debug("move update for cell {0}: {1} -> {2}", 
            cellIndex, cells[cellIndex].pos, newFieldIndex);
        moveCell(cellIndex, newFieldIndex);
    }
}

void StochasticRealisation::growthUpdate(const size_t& cellIndex) {
    // divide by 3 as all three actions are called in one update
    float sizeDiff = params.growthPercentage / 3.f * cells[cellIndex].size *
                    (1 - cells[cellIndex].size / params.growthMax);
    growCell(cellIndex, sizeDiff);
}

void StochasticRealisation::splitUpdate(const size_t& cellIndex) {
    float splitProbability = params.splitProbability * 
        exp(-pow(cells[cellIndex].size / params.splitMinSize, params.splitCurvature));
    // divide by 3 as all three actions are called in one update
    bool splitTrigger{stats::unifloat() < splitProbability / 3.f};
    if(splitTrigger) {
        // TODO implement ratio sampling: gauss(mean, std)
        splitCell(cellIndex, params.splitRatioMean); // insert here instead of 'params.splitRatioMean'
    }
}    

};