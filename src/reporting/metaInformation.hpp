#pragma once

#include "../std.hpp"

namespace CellularAutomata::Reporting {

    class MetaInformation {
        std::string header, content;
    public:
        MetaInformation(std::string header, std::string content);
        friend std::ostream& operator<<(std::ostream& out, const MetaInformation& m);
    };

    std::ostream& operator<<(std::ostream& out, const MetaInformation& m);

};