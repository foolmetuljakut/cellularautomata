#pragma once

#include <gtest/gtest.h>

#include "../../src/statistics/sum.hpp"

namespace CellularAutomata::Statistics {

    TEST(SumTest, SimpleSumTest) {
        std::array<float, 3> values{-1, 0, 1};
        ASSERT_EQ(sum<3>(values), 0.f);
    }

};