#include "std.hpp"

#include "statistics.hpp"
namespace stats = CellularAutomata::Statistics;
#include "scenarios.hpp"
using namespace CellularAutomata::Simulation;
using namespace CellularAutomata::Scenarios;
#include "reporting.hpp"
using namespace CellularAutomata::Reporting;
#include "gui.hpp"
using namespace CellularAutomata::Gui;

void exportState(const StochasticRealisation& r, Reason& timeSeries, std::string dirName) {
    Reason currentState(dirName);

    currentState.addClaim(MetaInformation("max_cell_per_field: prove colony size transits from exponentially to quadratic", 
    "colony size grows exponential initially due to cells doubling. If after a while the colony is massive, "
    "only the rim (proportional to it's circumference/surface) is contributing to growth"));
    currentState.addFieldEnsembleMap("max_cell_per_field", [&r](const std::vector<Field>& fields) {
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

    currentState.addClaim(MetaInformation("cell_size_bins: prove that cell size distribution becomes stationary", 
    "As cells grow but tend to split after reaching a certain size, their size distribution will "
    "stabilize around a certain size with a certain size distribution"));
    currentState.addCellEnsembleMap("cell_size_bin_bounds", [](const std::vector<Cell>& cells) {
        return stats::range(11, 0.f, 10.f);
    });
    currentState.addCellEnsembleMap("cell_size_bins", [](const std::vector<Cell>& cells) {
        std::vector<float> samples = stats::map<float, Cell>(cells, [](const Cell& cell){ return cell.size; });
        std::vector<float> binBounds = stats::range(11, 0.f, 10.f);
        std::vector<float> binning = stats::bin(samples, binBounds);
        return binning;
    });

    timeSeries.process(r);
    timeSeries.save(r);
    currentState.process(r);
    currentState.save(r);
}

Reason timeSeriesRecording() {
    Reason timeSeries("time_series");

    timeSeries.addClaim(MetaInformation("total_colony_size: prove colony size transits from exponentially to quadratic", 
    "colony size grows exponential initially due to cells doubling. If after a while the colony is massive, "
    "only the rim (proportional to it's circumference/surface) is contributing to growth"));
    timeSeries.addCellEnsembleStatMap("total_colony_size", [](const std::vector<Cell>& cells) {
        std::vector<float> samples = stats::map<float, Cell>(cells, [](const Cell& cell){ return cell.size; });
        float s = stats::sum(samples);
        return s;
    });
    timeSeries.addCellEnsemblePlot("total_colony_size", TimeSeriesUncertaintyPlot(
        "total colony size", "std", 1.f / 24.f, 1.f, "t / d", "N [cells]", "", false, true
    ));

    return timeSeries;
}

void exportTimeSeries(const StochasticRealisation& r, Reason& timeSeries) {
    timeSeries.process(r);
    timeSeries.save(r);
}

int main(int argc, char** argv) {

    spdlog::set_level(spdlog::level::info); // Set global log level to debug
    Application app("2D cell simulation", 200, 150, 100.f, 15);
    app.run();

    return 0;
}