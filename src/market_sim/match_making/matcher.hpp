#pragma once

#include "../../std.hpp"

#include "../order.hpp"
#include "../account.hpp"

#include "match.hpp"

namespace CellularAutomata::MarketSim::MatchMaking {

    class Matcher {
    public:
        
        virtual std::tuple<Matches, Matches> findMatches(
            std::vector<Order> openPurchases, 
            std::vector<Order> openSales
        ) = 0;
    };

};