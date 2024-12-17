#pragma once

#include "../std.hpp"

#include "field.hpp"
#include "cell.hpp"

namespace CellularAutomata::Simulation {

    class Realisation {
    
        void placeCell(size_t cellIndex, size_t newFieldIndex);
        void moveCell(size_t cellIndex, size_t newFieldIndex);

    public:
        std::vector<Field> fields;
        std::vector<Cell> cells;

        Realisation(size_t width, size_t height, size_t numberOfCells);

        void initialize(std::function<size_t()> position_index_generator);
    };

};