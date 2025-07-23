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

    std::cout << "purchase matches: " << std::endl;
    for(auto& [p_key, p_list] : purchaseMatches) {
        std::cout << p_key << " -> ";
        for(auto& m : p_list) {
            std::cout << m << ",";
        }
        std::cout << std::endl;
    }
    std::cout << "sale matches: " << std::endl;
    for(auto& [s_key, s_list] : saleMatches) {
        std::cout << s_key << " -> ";
        for(auto& m : s_list) {
            std::cout << m << ",";
        }
        std::cout << std::endl;
    }
}