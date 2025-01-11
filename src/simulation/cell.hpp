#pragma once

#include "../std.hpp"

namespace CellularAutomata::Simulation {

    // TODO move this to somewhere more easier to find
    static size_t invalidIndex = std::numeric_limits<size_t>::max();

    class Cell {
        static size_t idCtr;

        size_t _id;

    public:
        size_t pos;
        float size;
        bool active;

        Cell(): _id{idCtr++}, pos{invalidIndex}, size{0}, active{true} {}
        Cell(size_t fieldIndex, float size): _id{idCtr++}, pos{fieldIndex}, size{size}, active{true} {}

        size_t id() const { return _id; }

        friend std::ostream& operator<<(std::ostream& out, const Cell& cell);
    };

    std::ostream& operator<<(std::ostream& out, const Cell& cell);

};