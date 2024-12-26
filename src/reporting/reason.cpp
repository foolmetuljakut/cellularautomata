#include "reason.hpp"

namespace CellularAutomata::Reporting {

    void Reason::createWorkspace(std::string name) {
        if(!std::filesystem::exists(std::filesystem::directory_entry(name))) {
            std::filesystem::create_directory(name);
        }
    }

    void Reason::createRawSpace() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream dateTime;
        dateTime << std::put_time(&tm,"%Y_%m_%d-%H_%M_%S");
        rawDataSpace = workspace / ("raw_" + dateTime.str());
        createWorkspace(rawDataSpace);
    }

    void Reason::streamClaim() {
        std::ofstream out(workspace / "claims.csv");
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

            auto mapDir = workspace / "cellMaps";
            createWorkspace(mapDir);

            std::ofstream out(mapDir / (name + ".csv"));
            out << name << std::endl;
            std::vector<float> data;
            data.reserve(r.cells.size());
            for(const auto& cell: r.cells) {
                float value = mapping(cell);
                out << value << std::endl;
                data.push_back(value);
            }
            out.close();

            if(cellMapData.find(name) != cellMapData.end()) {
                for(const auto& value: data) {
                    cellMapData[name].push_back(value);
                }
            } else {
                cellMapData.insert({name, data});
            }
        }
    }

    void Reason::streamIndividualFieldData(const scen::StochasticRealisation& r) {
        for(auto& [name, mapping] : fieldMaps) {

            auto mapDir = workspace / "fieldMaps";
            createWorkspace(mapDir);

            std::ofstream out(mapDir / (name + ".csv"));
            out << name << std::endl;
            std::vector<float> data;
            data.reserve(r.fields.size());
            for(const auto& field: r.fields) {
                float value = mapping(field);
                out << value << std::endl;
                data.push_back(value);
            }
            out.close();

            if(fieldMapData.find(name) != fieldMapData.end()) {
                for(const auto& value: data) {
                    fieldMapData[name].push_back(value);
                }
            } else {
                fieldMapData.insert({name, data});
            }
        }
    }
    
    void Reason::streamEnsembleCellData(const scen::StochasticRealisation& r) {
        for(auto& [name, mapping] : cellEnsembleMaps) {

            auto mapDir = workspace / "cellEnsembleMaps";
            createWorkspace(mapDir);

            std::ofstream out(mapDir / (name + ".csv"));
            out << name << std::endl;
            std::vector<float> data = mapping(r.cells);
            for(const auto& value: data) {
                out << value << std::endl;
            }
            out.close();

            if(cellEnsembleData.find(name) != cellEnsembleData.end()) {
                for(const auto& value: data) {
                    cellEnsembleData[name].push_back(value);
                }
            } else {
                cellEnsembleData.insert({name, data});
            }
        }
    }
    
    void Reason::streamEnsembleCellStatData(const scen::StochasticRealisation& r) {
        for(auto& [name, mapping]: cellStatMaps) {

            auto mapDir = workspace / "cellEnsembleMaps";
            createWorkspace(mapDir);

            std::ofstream out(mapDir / (name + ".csv"));
            out << name << std::endl;
            float data = mapping(r.cells);
            out << data << std::endl;;
            out.close();

            if(cellEnsembleData.find(name) != cellEnsembleData.end()) {
                cellEnsembleData[name].push_back(data);
            } else {
                cellEnsembleData.insert({name, std::vector<float>{data}});
            }

        }
    }

    void Reason::streamEnsembleFieldData(const scen::StochasticRealisation& r) {
        for(auto& [name, mapping] : fieldEnsembleMaps) {

            auto mapDir = workspace / "fieldEnsembleMaps";
            createWorkspace(mapDir);

            std::ofstream out(mapDir / (name + ".csv"));
            out << name << std::endl;
            std::vector<float> data = mapping(r.fields);
            for(const auto& value: data) {
                out << value << std::endl;
            }
            out.close();

            if(fieldEnsembleData.find(name) != fieldEnsembleData.end()) {
                for(const auto& value: data) {
                    fieldEnsembleData[name].push_back(value);
                }
            } else {
                fieldEnsembleData.insert({name, data});
            }
        }
    }

    void Reason::streamProcessedData(const scen::StochasticRealisation& r) {
        streamIndividualCellData(r);
        streamIndividualFieldData(r);
        streamEnsembleCellData(r);
        streamEnsembleCellStatData(r);
        streamEnsembleFieldData(r);
    }

    Reason::Reason(std::string workspace) : workspace{workspace} {
        createWorkspace(workspace);
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

    void Reason::addClaim(MetaInformation&& m) {
        claim.push_back(std::move(m));
    }

    std::string Reason::getWorkspaceDir() {
        return workspace.string();
    }

    std::string Reason::getRawDataSpace() {
        return rawDataSpace.string();
    }

    Reason& Reason::operator<<(const scen::StochasticRealisation& r) {
        /*pipe realisation through all extractors and create timePlots & distroPlots 
        pipe all raw data, tracking information & plots into workspace*/
        streamClaim();
        createRawSpace();

        streamRawData(r);
        streamProcessedData(r);

        return *this;
    }

};