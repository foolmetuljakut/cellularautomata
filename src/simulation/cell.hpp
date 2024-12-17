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

        Cell(): _id{idCtr++}, pos{invalidIndex}, size{0} {}
        Cell(size_t fieldIndex, float size): _id{idCtr++}, pos{fieldIndex}, size{size} {}

        size_t id() { return _id; }
    };

};