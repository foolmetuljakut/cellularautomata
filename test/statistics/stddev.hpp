#pragma once

#include <gtest/gtest.h>

#include "../../src/statistics/mean.hpp"
#include "../../src/statistics/stddev.hpp"

namespace CellularAutomata::statistics {

TEST(StdTests, TestStandardDeviation) {
    std::array<float, 3> values{-1, 0, 1};
    float m = mean<3>(values);
    int _7digits = static_cast<int>(stddev<3>(values, m) * 1e7);
    int _7digits_ref = static_cast<int>(1 * 1e7);
    ASSERT_EQ(_7digits, _7digits_ref);
}

};