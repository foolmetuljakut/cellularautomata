#include <gtest/gtest.h>

#include "scenarios.hpp"
#include "simulation.hpp"
#include "statistics.hpp"
#include "reporting.hpp"

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::critical);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}