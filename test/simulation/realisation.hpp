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

    TEST(RealisationTests, ParametrizationSetTest) {
        Parametrization p(
            1.f, // 1s
            0.001f, // 1mm
            0.001f, // 1mm/s
            static_cast<float>(1.1574e-11), // (1 millimeter²) / day ≈ 1.157407407E−11 m²/s
            static_cast<float>(1.1574e-5), // 1 / day ≈ 0.00001157407407 s^−1
            0.25f, // 2/3rds of split events occur within +/- 0.25 of the mean
            0.5f // split 50/50
        );

        ASSERT_FLOAT_EQ(p.movementProbability, 1.f);
        ASSERT_FLOAT_EQ(p.growthPercentage, 1.1574e-5f);
        ASSERT_FLOAT_EQ(p.splitProbability, 1.1574e-5f);
    }

};