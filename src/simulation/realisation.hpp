#pragma once

#include "../std.hpp"

#include "field.hpp"
#include "cell.hpp"

namespace CellularAutomata::Simulation {

    class Realisation {
    
    public:
        std::vector<Field> fields;
        std::vector<Cell> cells;

        Realisation(size_t width, size_t height, size_t numberOfCells) {
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
    };

};