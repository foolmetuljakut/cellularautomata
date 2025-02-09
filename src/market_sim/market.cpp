#include "market.hpp"

namespace CellularAutomata::MarketSim {

    size_t Market::addOrder(Order&& order, std::vector<Order>& orderBook, size_t actorId) {
        orderBook.emplace_back(std::move(order));
        size_t orderId = orderBook.size() - 1;
        orderBook[orderId].actorId = actorId;
        return orderId;
    }

    size_t Market::addOrder(Order&& order, TransactionType type, size_t actorId) {
        switch(type) {
            case TransactionType::BUY:
                return addOrder(std::move(order), openPurchases, actorId);
            case TransactionType::SELL:
                return addOrder(std::move(order), openSales, actorId);
            default:
                throw "can't be reached as TransactionType is exhausted";
        }
    }

    size_t Market::findMatch(Order order, std::vector<Order>& matchPool) {
        return 0;
    }

    void Market::transact(Order& buy, Order& sell, size_t amount, float agreedPrice) {
        float totalSum = amount * agreedPrice;
        Account buyer = actors[buy.actorId], seller = actors[sell.actorId];
        
        buyer.money -= totalSum; // buyer pays money
        buyer.units += amount; // buyer gets units in return
        seller.money += totalSum; // seller gets money
        seller.units -= amount; // seller gives up units in return

        buy.amount -= amount; // buying order of what's asked for is reduced by whats received
        sell.amount -= amount; // selling order of whats available is reduced by whats given
    }

    TransactionType Market::decidePriceMakingParty(Order& buy, Order& sell) {
        // TODO make actual decision
        return TransactionType::BUY;
    }

    size_t Market::addActor(float initialMoney, size_t initialUnits) {
        actors.emplace_back(Account{initialMoney, initialUnits});
        return actors.size() - 1;
    }

    void Market::updatePurchase(size_t id) {
        Order purchase = openPurchases[id];
        size_t matchId = findMatch(purchase, openSales);
        if(matchId >= openSales.size()) {
            return;
        }
        Order match = openSales[matchId];

        TransactionType decidingParty = decidePriceMakingParty(purchase, match);
        float unitPrice = decidingParty == TransactionType::BUY ? purchase.unitPrice : match.unitPrice;
        size_t unitsTransacted = purchase.amount < match.amount ? purchase.amount : match.amount;
        transact(purchase, match, unitsTransacted, unitPrice);
        
        if(purchase.amount < match.amount) {
            openPurchases.erase(openPurchases.begin() + id);
        } else if(match.amount < purchase.amount) {
            openSales.erase(openSales.begin() + matchId);
        } else { // amounts equal
            openPurchases.erase(openPurchases.begin() + id);
            openSales.erase(openSales.begin() + matchId);
        }
    }
};