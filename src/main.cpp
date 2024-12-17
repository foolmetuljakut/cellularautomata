#include "std.hpp"

#include "scenarios.hpp"
using namespace CellularAutomata::Simulation;
using namespace CellularAutomata::Scenarios;

void exportState(const StochasticRealisation& r, std::string filename) {
    std::ofstream out(filename);

    out << "cells:\n";
    for (auto& cell : r.cells) {
        out << "{ " << cell.pos << ", " << cell.size << " }\n";
    }

    out.close();
}

int main(int argc, char** argv) {

    spdlog::set_level(spdlog::level::debug); // Set global log level to debug

    StochasticRealisation r = SettingsBuilder().standardSetting().build();

    exportState(r, "s_init.state");
    for (size_t t = 0; t < 2; t++) {
        r.update();

        std::stringstream s;
        s << "s" << t << ".state";
        exportState(r, s.str());
    }

    return 0;
}