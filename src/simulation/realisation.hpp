#pragma once

#include "../std.hpp"

#include "../statistics.hpp"
namespace stats = CellularAutomata::Statistics;

#include "field.hpp"
#include "cell.hpp"
#include "parametrization.hpp"

namespace CellularAutomata::Simulation {

    class Realisation {
    protected:
        virtual void placeCell(const size_t& cellIndex, const size_t& newFieldIndex);
        virtual void removeCell(const size_t& cellIndex, const size_t& oldFieldIndex);
        virtual bool isConsistent(const Cell& cell);

        size_t width, height;
        float fieldSize;
        bool unitProxySet;
        Parametrization params;

    public:
        std::vector<Field> fields;
        std::vector<Cell> cells;

        Realisation(size_t width, size_t height, float fieldSize, size_t numberOfCells);

        void initialize(std::function<void(Cell& cell)> cellInitializer);
        void setUnits(Parametrization params);
        Parametrization getUnits() { return params; }
        size_t getWidth() const { return width; }
        size_t getHeight() const { return height; }
        float getFieldSize() const { return fieldSize; }
    
        virtual void moveCell(const size_t& cellIndex, const size_t& newFieldIndex);
        virtual void growCell(const size_t& cellIndex, const float& amount);
        virtual void splitCell(const size_t& cellIndex, const float& splitRatio);
    };

};