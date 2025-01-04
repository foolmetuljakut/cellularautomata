#pragma once

#include "../std.hpp"

#include <sciplot/sciplot.hpp>

namespace CellularAutomata::Reporting {

    /*
    plan forward: 
        implement plotting class (TimeSeriesUncertaintyPlot)
            time, values, uncertainty (or deviation +/-)

        implement extractor, mapping data from a realisation to raw data sets

        implement information tracking class (MetaInformation)
            software version
            model version
            
        implement "argument folder" class (Reason) -> log a claim of what this data is supposed to show
            creating raw data, 
            logging software information (git tag or so),
            logging evaluated data,
            plotting results,
            required other argument-folders proving a different point, necessary here
    */

    class TimeSeriesUncertaintyPlot {
    public:
        std::string curveLabel, uncertaintyLabel;
        float xScale, yScale;
        std::string xlabel, ylabel, title;
        bool xlog, ylog;

        TimeSeriesUncertaintyPlot() {}
        TimeSeriesUncertaintyPlot(
        std::string curveLabel, std::string uncertaintyLabel,
        float xScale, float yScale,
        std::string xlabel, std::string ylabel, std::string title,
        bool xlog, bool ylog);
    };

};