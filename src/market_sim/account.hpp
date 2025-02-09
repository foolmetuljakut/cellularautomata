#pragma once

#include "../std.hpp"

namespace CellularAutomata::MarketSim {

    class Account {
    public:
        float money;
        size_t units;
        Account(float money, size_t units): money{money}, units{units} {}
    };
};