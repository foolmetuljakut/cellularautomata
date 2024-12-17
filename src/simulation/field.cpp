#include "field.hpp"

namespace CellularAutomata::Simulation {
    
    size_t Field::idCtr = 0;

    std::ostream& operator<<(std::ostream& out, const Field& field) {
        return out << "{" << field._id << "rem.space=" << field.remainingSpace << "/" << field.size 
                    << " @ (" << field.x << "," << field.y << ")}";
    }
};