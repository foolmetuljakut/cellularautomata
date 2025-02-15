#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/market_sim.hpp"
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

    ASSERT_EQ(market.getActorUnits(sellerId), 0);
    ASSERT_EQ(market.getActorUnits(buyerId), 1);
    ASSERT_EQ(market.getActorMoney(sellerId), 1.f);
    ASSERT_EQ(market.getActorMoney(buyerId), 0.f);
}