#pragma once

#include <gtest/gtest.h>

#include "../../src/statistics/minmax.hpp"

namespace CellularAutomata::Statistics {

TEST(MinMaxTest, SimpleMinTest) {
    std::vector<float> values{-1, 0, 1};
    ASSERT_EQ(min(values), -1.f);
}

TEST(MinMaxTest, SimpleMaxTest) {
    std::vector<float> values{-1, 0, 1};
    ASSERT_EQ(max(values), 1.f);
}

};