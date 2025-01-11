#pragma once

#include "../std.hpp"

#include "../scenarios.hpp"
namespace scen = CellularAutomata::Scenarios;

#include "metaInformation.hpp"
#include "extractor.hpp"
#include "timeSeries.hpp"
#include "distribution.hpp"

#include <sciplot/sciplot.hpp>

namespace CellularAutomata::Reporting {

    class Reason {
        std::filesystem::path workspace, rawDataSpace, procDataSpace;
        
        std::map<std::string, IndividualCellExtractor> cellMaps;
        std::map<std::string, IndividualPostProcessor> cellPostProcessingMaps;
        std::map<std::string, CollectionCellStatMap> cellStatMaps;
        std::map<std::string, IndividualPostProcessor> cellPostProcessingStatMaps;

        std::map<std::string, IndividualFieldExtractor> fieldMaps;
        std::map<std::string, IndividualPostProcessor> fieldPostProcessingMaps;

        std::map<std::string, CollectionCellExtractor> cellEnsembleMaps;
        std::map<std::string, CollectionPostProcessor> cellEnsemblePostProcessingMaps;
        std::map<std::string, TimeSeriesUncertaintyPlot> cellEnsemblePlots;

        std::map<std::string, CollectionFieldExtractor> fieldEnsembleMaps;
        std::map<std::string, CollectionPostProcessor> fieldEnsemblePostProcessingMaps;

        std::map<std::string, std::vector<float>> cellMapData, fieldMapData, cellEnsembleData, fieldEnsembleData;

        std::vector<TimeSeriesUncertaintyPlot> timePlots;
        std::vector<DistributionPlot> distroPlots;

        std::vector<MetaInformation> claim;

        void createWorkspace(std::string name);
        void streamClaim();
        void streamRawData(const scen::StochasticRealisation& r);

        void processIndividualCellData(const scen::StochasticRealisation& r);
        void streamIndividualCellData();

        void processIndividualFieldData(const scen::StochasticRealisation& r);
        void streamIndividualFieldData();

        void processEnsembleCellData(const scen::StochasticRealisation& r);
        void streamEnsembleCellData();
        void plotEnsembleCellData();

        void processEnsembleCellStatData(const scen::StochasticRealisation& r);
        void streamEnsembleCellStatData();

        void processEnsembleFieldData(const scen::StochasticRealisation& r);
        void streamEnsembleFieldData();

        void processData(const scen::StochasticRealisation& r);
        void streamProcessedData();

        void plotProcessedData();
    public:

        Reason(std::string workspace);
        void addCellMap(std::string name, IndividualCellExtractor e);
        void addFieldMap(std::string name, IndividualFieldExtractor e);
        void addCellEnsembleMap(std::string name, CollectionCellExtractor e);
        void addCellEnsembleStatMap(std::string name, CollectionCellStatMap e);
        void addFieldEnsembleMap(std::string name, CollectionFieldExtractor e);

        // void addCellPostProcessing(std::string name, IndividualPostProcessor e);
        // void addFieldPostProcessing(std::string name, IndividualPostProcessor e);
        // void addCellEnsemblePostProcessing(std::string name, CollectionPostProcessor e);
        // void addFieldEnsemblePostProcessing(std::string name, CollectionPostProcessor e);

        void addCellEnsemblePlot(std::string name, TimeSeriesUncertaintyPlot plotDescription);

        /*
        datenfluss:
            extraktoren -> post prozessoren -> plot accessoren 
            plot accessor: vector<string> of keys of data to be plotted
        */

        void addClaim(MetaInformation&& m);

        std::string getWorkspaceDir();
        std::string getRawDataSpace();
        std::string getProcDataSpace();

        void process(const scen::StochasticRealisation& r);
        void save(const scen::StochasticRealisation& r);

        void clear();
        std::vector<float> getCellMapData(std::string name) const;
        std::vector<float> getFieldMapData(std::string name) const;
        std::vector<float> getCellEnsembleData(std::string name) const;
        std::vector<float> getFieldEnsembleData(std::string name) const;
    };
};