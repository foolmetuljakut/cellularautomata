#pragma once

#include <gtest/gtest.h>

#include "../../src/statistics/range.hpp"
#include "../../src/statistics/bin.hpp"

namespace CellularAutomata::Statistics {

    TEST(BinTest, SimpleBinTest) {
        std::vector<float> samples{ -0.5f, // outside
                                    1, 
                                    2, 2.5f,
                                    3, // equal to upper bound -> outside
                                    3.5f // outside
                                  };
        std::array<float, 4> bounds {0, 1, 2, 3};
        std::array<float, 3> histogram = bin<3>(samples, bounds);
        
        ASSERT_EQ(histogram[0], 0.f);
        ASSERT_EQ(histogram[1], 1.f);
        ASSERT_EQ(histogram[2], 2.f);
    }

};