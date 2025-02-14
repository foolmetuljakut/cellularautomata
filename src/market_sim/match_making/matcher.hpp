#pragma once

#include "../../std.hpp"

#include "../order.hpp"
#include "../account.hpp"

#include "match.hpp"

namespace CellularAutomata::MarketSim::MatchMaking {

    class Matcher {
    public:
        
        virtual std::tuple<Match, Match> findMatches(
            std::vector<Order> openPurchases, 
            std::vector<Order> openSales
        ) = 0;
    };

};