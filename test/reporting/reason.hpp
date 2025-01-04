#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/statistics.hpp"
#include "../../src/simulation.hpp"
namespace sim = CellularAutomata::Simulation;
#include "../../src/scenarios.hpp"
namespace scen = CellularAutomata::Scenarios;
#include "../../src/reporting.hpp"
namespace rep = CellularAutomata::Reporting;

namespace CellularAutomata::Scenarios {

class TestReasoning: public testing::Test {

    void cleanUp() {
        if(std::filesystem::exists(std::filesystem::directory_entry(workspaceDirName))) {
            std::filesystem::remove_all(workspaceDirName);
        }
    }

protected:
    std::string workspaceDirName;

    TestReasoning(): workspaceDirName{"workspace"} {
        cleanUp();
    }
    ~TestReasoning() {
        cleanUp();
    }
};

TEST_F(TestReasoning, TestWorkspaceCreation) {
    rep::Reason r(workspaceDirName);
    ASSERT_TRUE(std::filesystem::exists(std::filesystem::directory_entry(workspaceDirName)));
};

TEST_F(TestReasoning, workspaceCreation) {
    rep::Reason reason(workspaceDirName);    
    ASSERT_TRUE(std::filesystem::exists(reason.getWorkspaceDir()));
    ASSERT_TRUE(std::filesystem::exists(reason.getRawDataSpace()));
    ASSERT_TRUE(std::filesystem::exists(reason.getProcDataSpace()));
}

TEST_F(TestReasoning, TestDataLogging) {

    rep::Reason reason(workspaceDirName);
    reason.addCellMap("position_indices", [](const sim::Cell& cell) { return (float)cell.pos; });

    scen::StochasticRealisation realisation = scen::SettingsBuilder().standardSetting().build();
    reason << realisation;

    std::filesystem::path wsPath(reason.getProcDataSpace());
    std::filesystem::path cellMapDir(wsPath / "cellMaps");

    ASSERT_TRUE(std::filesystem::exists(cellMapDir / "position_indices.csv"));
}

};