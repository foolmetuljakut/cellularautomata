#pragma once

#include "../std.hpp"

namespace CellularAutomata::MarketSim {

    enum class TransactionType { BUY, SELL };
    enum class OrderType { MARKET, LIMIT, STOP };

    class Order {
    public:
        size_t amount;
        float unitPrice;
        size_t actorId;
        OrderType transaction;

        Order(size_t amount, float unitPrice, OrderType transaction) :
            amount{amount}, unitPrice{unitPrice}, actorId{0}, transaction{transaction} {}
    };

    /*
    plan for this project:
        implement three types of orders: market / limit / stop order
        distribution of buyers and sellers at different prices
        match market & limit orders to conduct trade
        depict development of price and total trade volume via sciplot

        simulate change in scarcity by shrinking the volume offered by buyers and sellers respectively
        write simple ai to change prices simply by:
            - try to find a buyer
                raise price if successful else lower price
            - try to find a seller
                lower price if successful else raise price
            and then place a new order
     */
};