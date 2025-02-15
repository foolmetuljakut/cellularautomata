#pragma once

#include "../../std.hpp"

#include "../order.hpp"
#include "../account.hpp"

#include "match.hpp"
#include "matcher.hpp"

namespace CellularAutomata::MarketSim::MatchMaking {

    class MedianMatcher: public Matcher {
        std::vector<size_t> findMatchesForPurchase(
            const Order& openPurchase, 
            std::vector<Order>& openSales
        );
        std::vector<size_t> findMatchesForSale(
            std::vector<Order>& openPurchases, 
            const Order& openSale
        );
        std::vector<size_t> range(size_t size);
        
    public:
        std::tuple<Matches, Matches> findMatches(
            std::vector<Order> openPurchases, 
            std::vector<Order> openSales
        );
    };

};