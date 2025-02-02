#include "stochasticRealisation.hpp"

namespace CellularAutomata::Scenarios {

StochasticRealisation::StochasticRealisation(size_t width, size_t height, float fieldSize, size_t numberOfCells)
    : Realisation(width, height, fieldSize, numberOfCells), 
    xDiff{-1, -1, -1,  0, 0,  1, 1, 1},
    yDiff{-1,  0,  1, -1, 1, -1, 0, 1} {

    }

size_t StochasticRealisation::neighbouringField(size_t fieldIndex, float r) {
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

bool StochasticRealisation::cellCanMove(const size_t& cellIndex) {

    // check if cell can move anywhere and stop asap if we know the answer
    size_t fieldIndex = cells[cellIndex].pos;
    if(fieldIndex == sim::invalidIndex) {
        return false;
    }

    auto& field = fields[fieldIndex];
    float x = field.x, y = field.y;

    for(size_t i = 0; i < xDiff.size(); i++) {
        float xd = x + xDiff[i], yd = y + yDiff[i];
        if(xd >= 0 && yd >= 0 && xd < width && yd < height) {
            size_t newFieldIndex = xd * height + yd;
            bool cellToFat{fields[newFieldIndex].remainingSpace < cells[cellIndex].size};
            if(!cellToFat) {
                return true;
            }
        }
    }

    return false;
}

bool StochasticRealisation::cellCanGrow(const size_t& cellIndex) {
    size_t fieldIndex = cells[cellIndex].pos;
    if(fieldIndex == sim::invalidIndex || fieldIndex >= fields.size()) {
        return false;
    }
    return fields[fieldIndex].remainingSpace > 0;
}

bool StochasticRealisation::cellCanSplit(const size_t& cellIndex) {
    return calcSplitProbability(cellIndex) > 0.05;
}

bool StochasticRealisation::determineActivity(const size_t& cellIndex) {
    if(cellCanMove(cellIndex)) {
        return true;
    }
    if(cellCanGrow(cellIndex)) {
        return true;
    }
    if(cellCanSplit(cellIndex)) {
        return true;
    }
    return false;
}

void StochasticRealisation::update() {
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

    spdlog::info("performing update step for {} cells with {} updates each", cellIndexSequence.size(), actionIndexSequence.size());
    size_t cellUpdateCounter = 0;
    // TODO track down bug: no update calls (growth should be happening always)
    std::uniform_int_distribution uniInt(0, 2);
    for(auto cellIndex : cellIndexSequence) {
        // is the cell already marked as inactive?
        if(!cells[cellIndex].active) {
            continue;
        }

        // can the cell be marked as inactive?
        if(!determineActivity(cellIndex)) {
            spdlog::debug("setting cell {} inactive", cellIndex);
            cells[cellIndex].active = false;
            continue;
        }

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
                throw "StochasticRealisation::update(): only three actions (0, 1, 2) supported.";
            }
        }
        cellUpdateCounter++;
    }
    spdlog::info("physics updates for {} ({}%) cells calculated", cellUpdateCounter, 
        100.f * static_cast<float>(cellUpdateCounter) / static_cast<float>(cells.size())
    );
}

StochasticRealisation& StochasticRealisation::update(size_t n_cycles) {
    for(size_t i = 0; i < n_cycles; i++) {
        update();
    }
    return *this;
}

void StochasticRealisation::moveUpdate(const size_t& cellIndex) {
    // divide by 3 as all three actions are called in one update
    float p = params.movementProbability / 3.f;
    bool moveTrigger{stats::unifloat() < p};
    spdlog::debug("movement prob: {}", p);
    if(moveTrigger) {
        size_t newFieldIndex = neighbouringField(cells[cellIndex].pos, stats::unifloat());
        moveCell(cellIndex, newFieldIndex);
    }
}

void StochasticRealisation::growthUpdate(const size_t& cellIndex) {
    // divide by 3 as all three actions are called in one update
    float sizeDiff = params.growthPercentage / 3.f * cells[cellIndex].size *
                    (1 - cells[cellIndex].size / params.growthMax);
    growCell(cellIndex, sizeDiff);
}

float StochasticRealisation::calcSplitProbability(const size_t& cellIndex) {
    // divide by 3 as all three actions are called in one update
    float splitProbability = params.splitProbability / 3.f * 
        exp(-pow(cells[cellIndex].size / params.splitMinSize, params.splitCurvature));
    return splitProbability;
}

void StochasticRealisation::splitUpdate(const size_t& cellIndex) {
    float splitProbability = calcSplitProbability(cellIndex);
    bool splitTrigger{stats::unifloat() < splitProbability};
    spdlog::debug("split probability: {}", splitProbability);
    if(splitTrigger) {
        // TODO implement ratio sampling: gauss(mean, std)
        splitCell(cellIndex, params.splitRatioMean); // insert here instead of 'params.splitRatioMean'
    }
}    

};