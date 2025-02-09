#pragma once

#include "../std.hpp"

#include "order.hpp"
#include "account.hpp"

namespace CellularAutomata::MarketSim {

    class Market {
        //Purchase = open order to buy, Sale = open oder to sell
        std::vector<Order> openPurchases, openSales;
        std::vector<Account> actors;

        size_t addOrder(Order&& order, std::vector<Order>& orderBook, size_t actorId);
        size_t findMatch(Order order, std::vector<Order>& matchPool);
        void transact(Order& buy, Order& sell, size_t amount, float agreedPrice);
        TransactionType decidePriceMakingParty(Order& buy, Order& sell);
    public:
        size_t addActor(float initialMoney, size_t initialUnits); 
        size_t addOrder(Order&& order, TransactionType type, size_t actorId);
        size_t openPurchaseOrders() { return openPurchases.size(); }
        size_t openSalesOrders() { return openSales.size(); }
        
        void updatePurchase(size_t id);
    };

};