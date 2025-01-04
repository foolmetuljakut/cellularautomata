#include "timeSeries.hpp"

namespace CellularAutomata::Reporting {

    TimeSeriesUncertaintyPlot::TimeSeriesUncertaintyPlot(
        std::string curveLabel, std::string uncertaintyLabel,
        float xScale, float yScale,
        std::string xlabel, std::string ylabel, std::string title,
        bool xlog, bool ylog)
        : curveLabel{curveLabel}, uncertaintyLabel{uncertaintyLabel},
        xScale{xScale}, yScale{yScale},
        xlabel{xlabel}, ylabel{ylabel}, title{title}, xlog{xlog}, ylog{ylog} {
        
    }
};