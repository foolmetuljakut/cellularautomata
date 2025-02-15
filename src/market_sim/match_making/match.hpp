#pragma once

#include "../../std.hpp"

#include "../order.hpp"
#include "../account.hpp"

namespace CellularAutomata::MarketSim::MatchMaking {

    typedef std::map<size_t, std::vector<size_t>> Matches;

};