#include "std.hpp"

#include "statistics.hpp"
namespace stats = CellularAutomata::Statistics;
#include "scenarios.hpp"
using namespace CellularAutomata::Simulation;
using namespace CellularAutomata::Scenarios;
#include "reporting.hpp"
using namespace CellularAutomata::Reporting;

void exportState(const StochasticRealisation& r, std::string dirName) {
    Reason reason(dirName);

    reason.addClaim(MetaInformation("total_colony_size: prove colony size transits from exponentially to quadratic", 
    "colony size grows exponential initially due to cells doubling. If after a while the colony is massive, "
    "only the rim (proportional to it's circumference/surface) is contributing to growth"));
    reason.addCellEnsembleStatMap("total_colony_size", [](const std::vector<Cell>& cells) {
        std::vector<float> samples = stats::map<float, Cell>(cells, [](const Cell& cell){ return cell.size; });
        float s = stats::sum(samples);
        return s;
    });

    reason.addClaim(MetaInformation("max_cell_per_field: prove colony size transits from exponentially to quadratic", 
    "colony size grows exponential initially due to cells doubling. If after a while the colony is massive, "
    "only the rim (proportional to it's circumference/surface) is contributing to growth"));
    reason.addFieldEnsembleMap("max_cell_per_field", [&r](const std::vector<Field>& fields) {
        return stats::map<float, Field>(fields, [&r](const Field& field){
            if(field.cellIndices.size() > 0) {
                // stats -> filter by predicate
                // access cells -> filter cells by field.cellIndices
                // -> get biggest cell size -> return 
                std::vector<float> cellSizes;
                for(const size_t& index: field.cellIndices) {
                    cellSizes.push_back(r.cells[index].size);
                }
                return stats::max(cellSizes);
            } else {
                return 0.f;
            }
        });
    });

    reason.addClaim(MetaInformation("cell_size_bins: prove that cell size distribution becomes stationary", 
    "As cells grow but tend to split after reaching a certain size, their size distribution will "
    "stabilize around a certain size with a certain size distribution"));
    reason.addCellEnsembleMap("cell_size_bin_bounds", [](const std::vector<Cell>& cells) {
        return stats::range(11, 0.f, 10.f);
    });
    reason.addCellEnsembleMap("cell_size_bins", [](const std::vector<Cell>& cells) {
        std::vector<float> samples = stats::map<float, Cell>(cells, [](const Cell& cell){ return cell.size; });
        std::vector<float> binBounds = stats::range(11, 0.f, 10.f);
        std::vector<float> binning = stats::bin(samples, binBounds);
        return binning;
    });

    reason << r;
}

int main(int argc, char** argv) {

    spdlog::set_level(spdlog::level::debug); // Set global log level to debug

    StochasticRealisation r = SettingsBuilder().standardSetting().build();

    exportState(r, "s_init_state");
    for (size_t t = 0; t < 2; t++) {
        r.update();

        std::stringstream s;
        s << "s" << t << "_state";
        exportState(r, s.str());
    }

    return 0;
}