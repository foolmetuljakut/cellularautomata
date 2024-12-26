#include "metaInformation.hpp"

namespace CellularAutomata::Reporting {

    MetaInformation::MetaInformation(std::string header, std::string content)
        : header{header}, content{content} {}

    std::ostream& operator<<(std::ostream& out, const MetaInformation& m) {
        out << m.header << "\n\n" << m.content << std::endl;
        return out;
    }

};