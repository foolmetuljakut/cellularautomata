#include "realisation.hpp"

namespace CellularAutomata::Simulation {

void Realisation::placeCell(size_t cellIndex, size_t newFieldIndex) {
    cells[cellIndex].pos = newFieldIndex;
    fields[newFieldIndex].cellIndices.push_back(cellIndex);
}

void Realisation::moveCell(size_t cellIndex, size_t newFieldIndex) {
    
    bool posIsInvalid{cells[cellIndex].pos == std::numeric_limits<size_t>::max()};
    if(!posIsInvalid) {
        size_t oldPos = cells[cellIndex].pos;

        auto& cellIndices = fields[oldPos].cellIndices;
        auto cellIndexIter = std::find(cellIndices.begin(), cellIndices.end(), cellIndex);
        if(cellIndexIter != cellIndices.end()) {
            fields[oldPos].cellIndices.erase(cellIndexIter);
        }
    }
    
    placeCell(cellIndex, newFieldIndex);
}

Realisation::Realisation(size_t width, size_t height, size_t numberOfCells) {
    fields.reserve(width*height);
    for(size_t i = 0; i < width; i++) {
        for(size_t j = 0; j < height; j++) {
            fields.emplace_back(Field(i, j));
        }
    }

    cells.reserve(numberOfCells);
    for(size_t i = 0; i < numberOfCells; i++) {
        cells.emplace_back(Cell());
    }
}

void Realisation::initialize(std::function<size_t()> position_index_generator) {
    for(size_t i = 0; i < cells.size(); i++) {
        size_t newFieldIndex = position_index_generator();
        placeCell(i, newFieldIndex);
    }
}

};