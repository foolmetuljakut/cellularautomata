#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/statistics.hpp"
#include "../../src/simulation.hpp"
using namespace CellularAutomata::Simulation;
#include "../../src/scenarios.hpp"

namespace CellularAutomata::Scenarios {

TEST(UutTests, TestDirectionDiffs) {
    
    StochasticRealisation r(1, 1, 10.f, 1);
    float xSum = stats::sum<8>(r.xDiff),
            ySum = stats::sum<8>(r.yDiff);
    ASSERT_EQ(xSum, 0);
    ASSERT_EQ(ySum, 0);
}

TEST(UutTests, TestNeighborhood) {

    StochasticRealisation r(3, 3, 10.f, 1);
    auto cellInitializer = [](Cell& cell){ cell.pos = 4; }; // center place of a 3x3
    r.initialize(cellInitializer);

    for(size_t i = 0; i <= 8; i++) {
        size_t fieldIndex = r.neighbouringField(4, static_cast<float>(i) / 8.f);

        if(i < 4) {
            ASSERT_EQ(fieldIndex, i);
        } else if(i < 8) {
            ASSERT_EQ(fieldIndex, i+1);
        } else {
            ASSERT_EQ(fieldIndex, i); // 8 is mapped to 7 in rounding down in 'neighbor'
        }
    }
}

TEST(UutTests, TestInvalidNeighborhoodSteps) {

    StochasticRealisation r(3, 3, 10.f, 1);
    auto cellInitializer = [](Cell& cell){ cell.pos = 0; }; // top left place of a 3x3
    r.initialize(cellInitializer);

    size_t invalids = 0;
    for(size_t i = 0; i <= 8; i++) {
        size_t fieldIndex = r.neighbouringField(0, static_cast<float>(i) / 8.f);

        if(fieldIndex == sim::invalidIndex) {
            invalids++;
        }
    }
    ASSERT_EQ(invalids, 5);
}

class MockStochasticRealisationUpdates: public IStochasticRealisation {
public:
    MockStochasticRealisationUpdates(size_t width, size_t height, float fieldSize, size_t numberOfCells)
        : IStochasticRealisation(width, height, fieldSize, numberOfCells) {}
    MOCK_METHOD(void, moveUpdate, (const size_t& cellIndex), (override));
    MOCK_METHOD(void, growthUpdate, (const size_t& cellIndex), (override));
    MOCK_METHOD(void, splitUpdate, (const size_t& cellIndex), (override));
};

TEST(UutTests, TestUpdateCalling) {
    MockStochasticRealisationUpdates r(1, 1, 0.f, 1);
    r.setUnits(CellularAutomata::Simulation::Parametrization(
            1.f, // 1s
            0.001f, // 1mm
            0.001f, // 1mm/s
            1.1574e-11f, // (1 millimeter²) / day ≈ 1.157407407E−11 m²/s
            1.1574e-5f, // 1 / day ≈ 0.00001157407407 s^−1
            0.25f, // 2/3rds of split events occur within +/- 0.25 of the mean
            0.5f // split 50/50
        )
    );
    EXPECT_CALL(r, moveUpdate(0)).Times(1);
    EXPECT_CALL(r, splitUpdate(0)).Times(1);
    EXPECT_CALL(r, growthUpdate(0)).Times(1);
    r.update();
}

};