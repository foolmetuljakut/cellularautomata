#pragma once

#include <gtest/gtest.h>

#include "../../src/statistics.hpp"
#include "../../src/simulation.hpp"

namespace CellularAutomata::Simulation {

    TEST(CellTests, InitTest) {
        /// Test if initial position is marked as invalid
        ASSERT_EQ(invalidIndex, std::numeric_limits<size_t>::max());

        Cell cell;
        ASSERT_EQ(cell.pos, invalidIndex);
        ASSERT_EQ(cell.size, 0.f);
    }

    TEST(FieldTests, InitTest) {
        Field field;
        ASSERT_EQ(field.cellIndices.size(), 0);
        ASSERT_EQ(field.x, 0.f);
        ASSERT_EQ(field.y, 0.f);
    }

    TEST(RealisationTests, CtorTest) {
        Realisation r(1, 1, 0, 3);

        ASSERT_EQ(r.fields.capacity(), 1);
        ASSERT_EQ(r.fields.size(), 1);
        ASSERT_EQ(r.fields[0].x, 0.f);
        ASSERT_EQ(r.fields[0].y, 0.f);

        ASSERT_EQ(r.cells.capacity(), 3);
        ASSERT_EQ(r.cells.size(), 3);
        for(size_t i = 0; i < 3; i++) {
            ASSERT_EQ(r.cells[i].pos, invalidIndex);
        }
    }

    TEST(RealisationTests, InitTest) {
        Realisation r(1, 1, 0, 3);
        auto cellInitializer = [](Cell& cell){ cell.pos = 0; };
        r.initialize(cellInitializer);
        ASSERT_EQ(r.cells[0].pos, 0);
        ASSERT_EQ(r.cells[1].pos, 0);
        ASSERT_EQ(r.cells[2].pos, 0);
    }

    TEST(RealisationTests, MoveTest) {
        Realisation r(1, 2, 0, 1);
        
        auto cellInitializer = [](Cell& cell){ cell.pos = 0; };
        r.initialize(cellInitializer);
        ASSERT_EQ(r.cells[0].pos, 0);

        size_t cellIndex = 0, newFieldIndex = 1;
        r.moveCell(cellIndex, newFieldIndex);
        ASSERT_EQ(r.cells[0].pos, 1);
        ASSERT_EQ(r.fields[0].cellIndices.size(), 0);
        ASSERT_EQ(r.fields[1].cellIndices.size(), 1);
    }

    TEST(RealisationTests, GrowTest) {
        Realisation r(1, 1, 10.f, 1);
        auto cellInitializer = [](Cell& cell){ cell.pos = 0; };
        r.initialize(cellInitializer);

        ASSERT_EQ(r.fields[0].size, 10.f);
        ASSERT_EQ(r.fields[0].remainingSpace, 10.f);
        ASSERT_EQ(r.cells[0].pos, 0);

        size_t cellIndex = 0;
        float growBySize = 6.f;
        
        r.growCell(cellIndex, growBySize);
        ASSERT_EQ(r.cells[0].size, 6.f);
        ASSERT_EQ(r.fields[0].remainingSpace, 4.f);
        
        r.growCell(cellIndex, growBySize);
        ASSERT_EQ(r.cells[0].size, 10.f);
        ASSERT_EQ(r.fields[0].remainingSpace, 0.f);
        
        r.growCell(cellIndex, growBySize);
        ASSERT_EQ(r.cells[0].size, 10.f);
        ASSERT_EQ(r.fields[0].remainingSpace, 0.f);
    }

    TEST(RealisationTests, SplitTest) {
        Realisation r(1, 1, 10.f, 1);
        auto cellInitializer = [](Cell& cell){ cell.pos = 0; };
        r.initialize(cellInitializer);
        ASSERT_EQ(r.cells.size(), 1);

        size_t cellIndex = 0;
        r.growCell(cellIndex, 10.f);
        ASSERT_EQ(r.cells[0].size, 10.f);

        r.splitCell(cellIndex, 0.5f);
        ASSERT_EQ(r.cells.size(), 2);
        ASSERT_EQ(r.cells[0].size, 5.f);
        ASSERT_EQ(r.cells[1].size, 5.f);
    }

    TEST(RealisationTests, ParametrizationImplicitParametersTest) {
        float time = 2, length = 3;
        Parametrization p(
            time, length, // time , length
            4, // v(move) 
            5, 6, // r(growth), A(growth)
            7, 8, 9, 10, 11 // r(split), A(split), m(split), s(split), M(split)
        );

        ASSERT_FLOAT_EQ(p.movementProbability, 4.f * time / length);

        ASSERT_FLOAT_EQ(p.growthPercentage, 5.f * time);

        ASSERT_FLOAT_EQ(p.splitProbability, 7.f * time);
    }

};