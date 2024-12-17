#pragma once

#include <gtest/gtest.h>

#include "../../src/statistics/range.hpp"

namespace CellularAutomata::Statistics {

    TEST(RangeTest, SimpleRangeTest) {
        // TODO seg fault in this test
        std::vector<size_t> values = range(3);
        for(size_t i = 0; i < 3; i++) {
            ASSERT_EQ(i, values[i]);
        } 
    }

    TEST(RangeTest, BoundedRangeTest) {
        std::vector<float> values = range(3, 1, 3);
        for(size_t i = 1; i <= 3; i++) { // from 1 -> 3, not 0 -> 2
            ASSERT_EQ(static_cast<float>(i), values[i-1]);
        } 
    }

};