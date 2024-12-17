#pragma once

#include <gtest/gtest.h>

#include "../../src/statistics/minmax.hpp"

namespace CellularAutomata::Statistics {

TEST(MinMaxTest, SimpleMinTest) {
    std::array<float, 3> values{-1, 0, 1};
    ASSERT_EQ(min<3>(values), -1.f);
}

TEST(MinMaxTest, SimpleMaxTest) {
    std::array<float, 3> values{-1, 0, 1};
    ASSERT_EQ(max<3>(values), 1.f);
}

};