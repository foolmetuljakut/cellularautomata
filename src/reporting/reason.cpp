#include "reason.hpp"

namespace CellularAutomata::Reporting {

    void Reason::createWorkspace(std::string name) {
        if(!std::filesystem::exists(std::filesystem::directory_entry(name))) {
            std::filesystem::create_directory(name);
        }
    }

    void Reason::streamClaim() {
        std::ofstream out(procDataSpace / "claims.csv");
        for(auto& m: claim) {
            out << m << "\n\n" << std::endl;
        }
        out.close();
    }

    void Reason::streamRawData(const scen::StochasticRealisation&  r) {
        {
            std::ofstream out(rawDataSpace / "cells.csv");
            out << "id,posIndex,size" << std::endl;
            for(const auto& cell: r.cells) {
                out << cell.id() << ", " << cell.pos << ", " << cell.size << std::endl;
            }
            out.close();
        }

        {
            std::ofstream out(rawDataSpace / "fields.csv");
            out << "x,y,size,remainingSpace,cellIndices" << std::endl;
            for(const auto& field: r.fields) {
                out << field.id() << ", " << field.x << ", " << field.y << ", " << field.size << ", " << field.remainingSpace << ", [";

                for(size_t i = 0; i < field.cellIndices.size(); i++) {
                    out << field.cellIndices[i] << (i < field.cellIndices.size() -1 ? ", " : "");
                }
                out << "]" << std::endl;
            }
            out.close();
        }
    }

    void Reason::streamIndividualCellData(const scen::StochasticRealisation& r) {
        for(auto& [name, mapping] : cellMaps) {

            auto mapDir = procDataSpace / "cellMaps";
            createWorkspace(mapDir);

            spdlog::debug("recording {}", name);
            std::vector<float> data;
            data.reserve(r.cells.size());
            for(const auto& cell: r.cells) {
                float value = mapping(cell);
                data.push_back(value);
            }

            if(cellMapData.find(name) != cellMapData.end()) {
                for(const auto& value: data) {
                    cellMapData[name].push_back(value);
                }
            } else {
                cellMapData.insert({name, data});
            }

            std::ofstream out(mapDir / (name + ".csv"));
            out << name << std::endl;
            for(const auto& value: cellMapData[name]) {
                out << value << std::endl;
            }
            out.close();
        }
    }

    void Reason::streamIndividualFieldData(const scen::StochasticRealisation& r) {
        for(auto& [name, mapping] : fieldMaps) {

            auto mapDir = procDataSpace / "fieldMaps";
            createWorkspace(mapDir);

            spdlog::debug("recording {}", name);
            std::vector<float> data;
            data.reserve(r.fields.size());
            for(const auto& field: r.fields) {
                float value = mapping(field);
                data.push_back(value);
            }

            if(fieldMapData.find(name) != fieldMapData.end()) {
                for(const auto& value: data) {
                    fieldMapData[name].push_back(value);
                }
            } else {
                fieldMapData.insert({name, data});
            }

            std::ofstream out(mapDir / (name + ".csv"));
            out << name << std::endl;
            for(const auto& value: fieldMapData[name]) {
                out << value << std::endl;
            }
            out.close();
        }
    }
    
    void Reason::streamEnsembleCellData(const scen::StochasticRealisation& r) {
        for(auto& [name, mapping] : cellEnsembleMaps) {

            auto mapDir = procDataSpace / "cellEnsembleMaps";
            createWorkspace(mapDir);

            spdlog::debug("recording {}", name);
            std::vector<float> data = mapping(r.cells);

            if(cellEnsembleData.find(name) != cellEnsembleData.end()) {
                for(const auto& value: data) {
                    cellEnsembleData[name].push_back(value);
                }
            } else {
                cellEnsembleData.insert({name, data});
            }

            std::ofstream out(mapDir / (name + ".csv"));
            out << name << std::endl;
            for(const auto& value: cellEnsembleData[name]) {
                out << value << std::endl;
            }
            out.close();
        }
    }

    void Reason::plotEnsembleCellData() {
        for(const auto& [name, data]: cellEnsembleData) {

            if(cellEnsemblePlots.find(name) == cellEnsemblePlots.end()) {
                continue;
            }

            TimeSeriesUncertaintyPlot descr = cellEnsemblePlots[name]; 

            size_t N = data.size();
            sciplot::Vec x = sciplot::linspace(0, N-1, N);

            sciplot::Plot2D plot;
            plot.xlabel(descr.xlabel);
            plot.ylabel(descr.ylabel);
            plot.legend()
                .title(descr.curveLabel)
                .atOutsideBottom()
                .displayHorizontal()
                .displayExpandWidthBy(2);

            if(descr.xlog) {
                plot.xtics().logscale(2);
            }
            if(descr.ylog) {
                plot.ytics().logscale(2);
            }


            std::vector<float> scaledData(data.size()), scaledTime;
            std::transform(
                data.begin(), data.end(), scaledData.begin(),
                [descr](float v) { return descr.yScale * v; }
            );
            for(size_t t = 0; t < data.size(); t++) {
                scaledTime.push_back(descr.xScale * t);
            }

            plot.drawCurve(scaledTime, scaledData).label(descr.title);

            sciplot::Figure fig{{plot}};
            sciplot::Canvas canvas{{fig}};

            auto mapDir = procDataSpace / "cellEnsembleMaps";
            std::stringstream s;
            s << mapDir << "/" << name << ".pdf";
            canvas.save(s.str());
        }
    }
    
    void Reason::streamEnsembleCellStatData(const scen::StochasticRealisation& r) {
        for(auto& [name, mapping]: cellStatMaps) {

            auto mapDir = procDataSpace / "cellEnsembleMaps";
            createWorkspace(mapDir);

            spdlog::debug("recording {}", name);
            float data = mapping(r.cells);

            if(cellEnsembleData.find(name) != cellEnsembleData.end()) {
                cellEnsembleData[name].push_back(data);
            } else {
                cellEnsembleData.insert({name, std::vector<float>{data}});
            }

            std::ofstream out(mapDir / (name + ".csv"));
            out << name << std::endl;
            for(const auto& value: cellEnsembleData[name]) {
                out << value << std::endl;
            }
            out.close();

        }
    }

    void Reason::streamEnsembleFieldData(const scen::StochasticRealisation& r) {
        for(auto& [name, mapping] : fieldEnsembleMaps) {

            auto mapDir = procDataSpace / "fieldEnsembleMaps";
            createWorkspace(mapDir);

            spdlog::debug("recording {}", name);
            std::vector<float> data = mapping(r.fields);

            if(fieldEnsembleData.find(name) != fieldEnsembleData.end()) {
                for(const auto& value: data) {
                    fieldEnsembleData[name].push_back(value);
                }
            } else {
                fieldEnsembleData.insert({name, data});
            }

            std::ofstream out(mapDir / (name + ".csv"));
            out << name << std::endl;
            for(const auto& value: fieldEnsembleData[name]) {
                out << value << std::endl;
            }
            out.close();
        }
    }

    void Reason::streamProcessedData(const scen::StochasticRealisation& r) {
        streamIndividualCellData(r);
        streamIndividualFieldData(r);
        streamEnsembleCellData(r);
        streamEnsembleCellStatData(r);
        streamEnsembleFieldData(r);
    }

    void Reason::plotProcessedData() {
        plotEnsembleCellData();
    }

    Reason::Reason(std::string workspace) : workspace{workspace} {
        
        createWorkspace(workspace);

        std::filesystem::path ws(workspace);

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream dateTime;
        dateTime << std::put_time(&tm,"%Y_%m_%d-%H_%M_%S");
        std::string dateTimeStr = dateTime.str();
        
        createWorkspace(ws / dateTimeStr);

        rawDataSpace = ws / dateTimeStr / "raw",
        procDataSpace = ws / dateTimeStr / "proc";

        createWorkspace(rawDataSpace);
        createWorkspace(procDataSpace);
    }

    void Reason::addCellMap(std::string name, IndividualCellExtractor e) {
        if(cellMaps.find(name) != cellMaps.end()) {
            throw "id already existing in reason (cellmaps)";
        }
        cellMaps.insert({name, e});
    }

    void Reason::addFieldMap(std::string name, IndividualFieldExtractor e) {
        if(fieldMaps.find(name) != fieldMaps.end()) {
            throw "id already existing in reason (fieldmaps)";
        }
        fieldMaps.insert({name, e});
    }

    void Reason::addCellEnsembleMap(std::string name, CollectionCellExtractor e) {
        if(cellEnsembleMaps.find(name) != cellEnsembleMaps.end()) {
            throw "id already existing in reason (cell ensemble)";
        }
        cellEnsembleMaps.insert({name, e});
    }

    void Reason::addCellEnsembleStatMap(std::string name, CollectionCellStatMap e) {
        if(cellStatMaps.find(name) != cellStatMaps.end()) {
            throw "id already existing in reason (cell stat maps)";
        }
        cellStatMaps.insert({name, e});
    }

    void Reason::addFieldEnsembleMap(std::string name, CollectionFieldExtractor e) {
        if(fieldEnsembleMaps.find(name) != fieldEnsembleMaps.end()) {
            throw "id already existing in reason (fields)";
        }
        fieldEnsembleMaps.insert({name, e});
    }

    void Reason::addCellEnsemblePlot(std::string name, TimeSeriesUncertaintyPlot plotDescription) {
        if(cellEnsemblePlots.find(name) != cellEnsemblePlots.end()) {
            throw "id already existing in reason (cell ensemble plots)";
        }
        cellEnsemblePlots.insert({name, plotDescription});
    }

    void Reason::addClaim(MetaInformation&& m) {
        claim.push_back(std::move(m));
    }

    std::string Reason::getWorkspaceDir() {
        return workspace.string();
    }

    std::string Reason::getRawDataSpace() {
        return rawDataSpace.string();
    }

    std::string Reason::getProcDataSpace() {
        return procDataSpace.string();
    }

    Reason& Reason::operator<<(const scen::StochasticRealisation& r) {
        /*pipe realisation through all extractors and create timePlots & distroPlots 
        pipe all raw data, tracking information & plots into workspace*/
        spdlog::info("recording reason");

        streamClaim();

        streamRawData(r);
        streamProcessedData(r);
        plotProcessedData();

        return *this;
    }

};