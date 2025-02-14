#pragma once

#include "../std.hpp"

#include "order.hpp"
#include "account.hpp"
/*
bunch of theory:
    - A market order simply buys or sells shares at the market price until your order is filled. You put in the order and you will get the next available price.
    - A limit order specifies the price you're willing to pay, though there is no guarantee that some or all of the order will trade if you haven't named a price other traders want.
    - Stop orders are a type of market order triggered when a stock moves above or below a certain price. Once it hits that limit, it buys or sells the stock. These are a key type of order for managing risk in the stock market.
    
    Order Type 	Description 	When to Place
    Buy Limit 	An order to buy a stock at or below a specific price. 	Place when you want to buy at a price lower than the current price.
    Sell Limit 	An order to sell a stock at or above a specific price. 	Place when you want to sell at a price higher than the current price.
    Buy Stop 	An order to buy a stock once its price reaches a particular point above the current market price. Once activated, it becomes a market or limit order. 	Place when you expect the stock price to keep rising after reaching a certain level.
    Sell Stop 	An order to sell a stock once its price reaches a specific level below the current market price. Once activated, it becomes a market or limit order. 	Place when you want to limit losses by selling if the price drops to a certain level.

    => rules:
        <sorted order lists>
        - market order: 
            repeatedly pick median actor => price of median actor = agreed price
            (find matches by picking median)
        - limit order:
            buy: pick everyone at maximum price and below, prioritize by closeness or FIFO-time 
                => price closest below limit = agreed price
            sell: pick everyone at minimum price and above, prioritize by closeness or FIFO-time
                => price closest above limit = agreed price
        - stop order:
            for these purposes a market or limit order activated above a limit (buy) or below (sell)
*/
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
        size_t getActorUnits(size_t actorId) { return actors[actorId].units; }
        float getActorMoney(size_t actorId) { return actors[actorId].money; }

        size_t addActor(float initialMoney, size_t initialUnits); 
        size_t addOrder(Order&& order, TransactionType type, size_t actorId);
        size_t openPurchaseOrders() { return openPurchases.size(); }
        size_t openSalesOrders() { return openSales.size(); }
        
        void updatePurchase(size_t id);
    };

};