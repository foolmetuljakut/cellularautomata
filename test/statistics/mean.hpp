#pragma once

#include <gtest/gtest.h>

#include "../../src/statistics/mean.hpp"

namespace CellularAutomata::statistics {

TEST(MeanTests, TestSummation) {
    std::array<float, 3> values{-1, 0, 1};
    ASSERT_EQ(mean<3>(values), 0.f);
}

};