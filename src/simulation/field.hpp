#pragma once

#include "../std.hpp"

namespace CellularAutomata::Simulation {

    class Field {
        static size_t idCtr;
        size_t _id;
    public:
        float x, y;
        std::vector<size_t> cellIndices;

        Field(): _id{idCtr++}, x{0}, y{0} {
            cellIndices.reserve(30); // most likely case: around 20-30 cells on a field
        }
        Field(float x, float y): _id{idCtr++}, x{x}, y{y} {
            cellIndices.reserve(30); // most likely case: around 20-30 cells on a field
        }

        size_t id() { return _id; }
    };

};