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
    r.setUnits(SettingsBuilder().standardParametrization());
    EXPECT_CALL(r, moveUpdate(0)).Times(1);
    EXPECT_CALL(r, splitUpdate(0)).Times(1);
    EXPECT_CALL(r, growthUpdate(0)).Times(1);
    r.update();
}

TEST(UutTests, TestProbabilities) {
    StochasticRealisation r = SettingsBuilder().standardSetting().build();
    Parametrization p = r.getUnits();
    
    ASSERT_FLOAT_EQ(p.deltaTime, 3600.f);
    ASSERT_FLOAT_EQ(p.deltaLength, 1e-6f);

    ASSERT_FLOAT_EQ(p.movementVelocity, 1.3888889e-8f);
    ASSERT_FLOAT_EQ(p.movementProbability, 50); // µm / h

    ASSERT_FLOAT_EQ(p.growthRate, 1.504630e-5f);
    ASSERT_FLOAT_EQ(p.growthPercentage, 0.054166682); // 1.3/d in 1/h

    ASSERT_FLOAT_EQ(p.splitRate, 1.5e-5f);
    ASSERT_FLOAT_EQ(p.splitProbability, 0.053999998f); // 1.3/d in 1/h

}

TEST(UutTests, TestGrowthUpdateAction) {

    StochasticRealisation r(1, 1, 10.f, 1);
    r.setUnits(SettingsBuilder().standardParametrization());
    auto cellInitializer = [](Cell& cell){ 
        cell.pos = 0;
        cell.size = 1.f;
    };
    r.initialize(cellInitializer);

    r.update(); // growth by < 5%
    ASSERT_GT(r.cells[0].size, 1.f);
    ASSERT_LT(r.cells[0].size, 1.05f);
}

};