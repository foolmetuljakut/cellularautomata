#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/market_sim.hpp"
using namespace CellularAutomata::MarketSim;
using namespace CellularAutomata::MarketSim::MatchMaking;

TEST(MatchMakingTests, SimpleMedianMatcherTest) {


    std::vector<Order> 
        openPurchases({Order(2, 1.f, OrderType::MARKET)}), 
        openSales({Order(1, 1.f, OrderType::MARKET), Order(1, 1.f, OrderType::MARKET)});

    MedianMatcher matcher;
    auto [purchaseMatches, saleMatches] = matcher.findMatches(openPurchases, openSales);

    std::cout << "purchase matches: " << purchaseMatches[0].size() << std::endl;
    std::cout << "sale matches: " << purchaseMatches[0][0] << ", " << purchaseMatches[1][0] << std::endl;
}