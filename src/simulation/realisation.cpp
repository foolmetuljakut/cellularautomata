#include "realisation.hpp"

namespace CellularAutomata::Simulation {

void Realisation::placeCell(const size_t& cellIndex, const size_t& newFieldIndex) {
    cells[cellIndex].pos = newFieldIndex;
    fields[newFieldIndex].cellIndices.push_back(cellIndex);
    float oldSpace = fields[newFieldIndex].remainingSpace;
    fields[newFieldIndex].remainingSpace -= cells[cellIndex].size;
    spdlog::debug("placing cell {0} at field {1} (space left: {2} -> {3})", 
        cells[cellIndex].id(), cells[cellIndex].pos, oldSpace, fields[newFieldIndex].remainingSpace);
}

void Realisation::removeCell(const size_t& cellIndex, const size_t& oldFieldIndex) {
    auto& cell = cells[cellIndex];
    auto& oldField = fields[oldFieldIndex];

    auto& cellIndices = oldField.cellIndices;
    auto cellIndexIter = std::find(cellIndices.begin(), cellIndices.end(), cellIndex);
    if(cellIndexIter != cellIndices.end()) {
        spdlog::debug("removing cell {0} from field {1} (space left: {2})", 
            cell.id(), cell.pos, oldField.remainingSpace);
        oldField.cellIndices.erase(cellIndexIter);
        oldField.remainingSpace += cell.size;
        cell.pos = invalidIndex;
    }
}

bool Realisation::isConsistent(const Cell& cell) {
    if(cell.pos > fields.size()) {
        throw "cell position inconsistent! (index > #fields)";
    }
    if(cell.pos == invalidIndex) {
        throw "cell position inconsistent! (invalid index)";
    }
    if(cell.size < 0) {
        throw "cell size inconsistent! (negative size)";
    }
    return true;
}

Realisation::Realisation(size_t width, size_t height, float fieldSize, size_t numberOfCells) 
    : width{width}, height{height}, fieldSize{fieldSize}
{
    fields.reserve(width*height);
    for(size_t i = 0; i < width; i++) {
        for(size_t j = 0; j < height; j++) {
            fields.emplace_back(Field(i, j, fieldSize));
        }
    }

    cells.reserve(numberOfCells);
    for(size_t i = 0; i < numberOfCells; i++) {
        cells.emplace_back(Cell());
    }
}

void Realisation::initialize(std::function<void(Cell& cell)> cellInitializer) {
    for(size_t i = 0; i < cells.size(); i++) {
        cellInitializer(cells[i]);
        placeCell(i, cells[i].pos);
        if(!isConsistent(cells[i])) {
            throw "consistency check for cells failed!";
        }
    }
}

void Realisation::setUnits(Parametrization params) {
    unitProxySet = true;
    params = params;
}

void Realisation::moveCell(const size_t& cellIndex, const size_t& newFieldIndex) {
    
    bool newIndexInvalid{newFieldIndex == invalidIndex};
    if(newIndexInvalid) {
        return;
    }

    auto& cell = cells[cellIndex];
    bool cellToFat{fields[newFieldIndex].remainingSpace < cell.size};
    if(cellToFat) {
        return;
    }

    bool posIsInvalid{cell.pos == invalidIndex};
    if(!posIsInvalid) {
        removeCell(cellIndex, cell.pos);
    }

    placeCell(cellIndex, newFieldIndex);
}

void Realisation::growCell(const size_t& cellIndex, const float& amount) {
    auto& cell = cells[cellIndex];
    size_t fieldIndex = cell.pos;
    
    bool fieldIndexInvalid {fieldIndex == invalidIndex};
    if(fieldIndexInvalid) {
        return;
    }

    auto& field = fields[fieldIndex];
    // less than this small delta not worth the calculation
    bool noSpaceAvailable{field.remainingSpace < 1e-7f};
    if(noSpaceAvailable) {
        return;
    }

    if(field.remainingSpace <= amount) {
        spdlog::debug("growing cell {0} from {1} by {2} (bounded)", cell.id(), cell.size, field.remainingSpace);
        cell.size += field.remainingSpace;
        field.remainingSpace = 0;
    } else {
        spdlog::debug("growing cell {0} from {1} by {2}", cell.id(), cell.size, amount);
        cell.size += amount;
        field.remainingSpace -= amount;
    }
}

void Realisation::splitCell(const size_t& cellIndex, const float& splitRatio) {
    
    cells.emplace_back(Cell());
    size_t sisterIndex = cells.size() - 1;
    placeCell(sisterIndex, cells[cellIndex].pos);
    spdlog::debug("splitting cell {0}, sister =", cells[cellIndex].id(), cells[sisterIndex].id());
    
    float sizeA = cells[cellIndex].size * splitRatio;
    float sizeB = cells[cellIndex].size - sizeA;
    cells[cellIndex].size = sizeA;
    cells[sisterIndex].size = sizeB;
}

};