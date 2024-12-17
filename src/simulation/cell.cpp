#include "cell.hpp"

namespace CellularAutomata::Simulation {
    
    size_t Cell::idCtr = 0;

    std::ostream& operator<<(std::ostream& out, const Cell& cell) {
        return out << "{" << cell._id << "size=" << cell.size
                    << " @ fIndex=" << cell.pos << "}";
    }
};