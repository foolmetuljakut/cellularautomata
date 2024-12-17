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
        ASSERT_EQ(cell.id(), 0);
        ASSERT_EQ(cell.size, 0.f);
    }

    TEST(FieldTests, InitTest) {
        Field field;
        ASSERT_EQ(field.cellIndices.size(), 0);
        ASSERT_EQ(field.x, 0.f);
        ASSERT_EQ(field.y, 0.f);
        ASSERT_EQ(field.id(), 0);
    }

    TEST(RealisationTests, InitTest) {
        Realisation r(1, 1, 3);

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

};