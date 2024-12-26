#pragma once

#include "../std.hpp"

namespace CellularAutomata::Simulation {

    class Field {
        static size_t idCtr;
        size_t _id;
    public:
        float x, y, size, remainingSpace;
        std::vector<size_t> cellIndices;

        Field(): _id{idCtr++}, x{0}, y{0}, size{0}, remainingSpace{0} {
            cellIndices.reserve(30); // most likely case: around 20-30 cells on a field
        }
        Field(float x, float y, float size): _id{idCtr++}, x{x}, y{y}, size{size}, remainingSpace{size} {
            cellIndices.reserve(30); // most likely case: around 20-30 cells on a field
        }

        size_t id() const { return _id; }

        friend std::ostream& operator<<(std::ostream& out, const Field& field);
    };

    std::ostream& operator<<(std::ostream& out, const Field& field);

};