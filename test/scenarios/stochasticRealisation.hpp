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

class MockStochasticRealisationUpdates: public StochasticRealisation {
public:
    MockStochasticRealisationUpdates(size_t width, size_t height, float fieldSize, size_t numberOfCells)
        : StochasticRealisation(width, height, fieldSize, numberOfCells) {}
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

class MockStochasticRealisationUpdateAction: public StochasticRealisation {
public:
    MockStochasticRealisationUpdateAction(size_t width, size_t height, float fieldSize, size_t numberOfCells)
        : StochasticRealisation(width, height, fieldSize, numberOfCells) {}
    /*Implementations of updates mocked above will call these actions*/
    MOCK_METHOD(void, moveCell, (const size_t& cellIndex, const size_t& newFieldIndex), (override));
    MOCK_METHOD(void, growCell, (const size_t& cellIndex, const float& amount), (override));
    MOCK_METHOD(void, splitCell, (const size_t& cellIndex, const float& splitRatio), (override));
};

TEST(UutTests, TestUpdateAction) {

    MockStochasticRealisationUpdateAction r(1, 1, 10.f, 1);
    auto parametrization = SettingsBuilder().standardParametrization();
    parametrization.splitRate = 1.f; /*some bullshit value, that ensures a call (probability of 1)*/
    parametrization.movementVelocity = 1.f; /*some bullshit value, that ensures a call (probability of 1)*/
    r.setUnits(parametrization);
    auto cellInitializer = [](Cell& cell){ 
        cell.pos = 0;
        cell.size = 1.f;
    };
    r.initialize(cellInitializer);

    size_t cellIndex = 0;

    float sizeDifference = 0.0180555601f;
    EXPECT_CALL(r, growCell(cellIndex, sizeDifference)).Times(1);

    size_t neighbouringIndex = invalidIndex;
    EXPECT_CALL(r, moveCell(cellIndex, neighbouringIndex)).Times(1);

    float splitRatio = 0.5f;
    EXPECT_CALL(r, splitCell(cellIndex, splitRatio)).Times(1);

    r.update();
}

};