#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/market_sim/market.hpp"
using namespace CellularAutomata::MarketSim;

TEST(MarketSim, TransactionTestBuy) {
    Market market;

    size_t buyerId = market.addActor(1.f, 0); // buyer
    size_t sellerId = market.addActor(0.f, 1); // seller

    size_t purchaseId = market.addOrder(Order(1, 1.f, OrderType::MARKET), TransactionType::BUY, buyerId);
    market.addOrder(Order(1, 1.f, OrderType::MARKET), TransactionType::SELL, sellerId);

    market.updatePurchase(purchaseId);

    ASSERT_EQ(market.openPurchaseOrders(), 0);
    ASSERT_EQ(market.openSalesOrders(), 0);
}