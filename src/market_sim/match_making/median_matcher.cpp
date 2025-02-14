#include "median_matcher.hpp"

namespace CellularAutomata::MarketSim::MatchMaking {


std::vector<size_t> MedianMatcher::findMatchesForPurchase(
    const Order& openPurchase, 
    std::vector<Order>& openSales) {

}

std::vector<size_t> MedianMatcher::findMatchesForSale(
    std::vector<Order>& openPurchases, 
    const Order& openSale) {

}

std::vector<size_t> MedianMatcher::range(size_t size) {
    std::vector<size_t> indices(size, 0);
    for(size_t i = 0; i < indices.size(); i++) {
        indices[i] = i;
    }
    return indices;
}

std::tuple<Match, Match> MedianMatcher::findMatches(
    std::vector<Order> openPurchases, 
    std::vector<Order> openSales) {
    // intentionally takes copies of data to shrink lists iteratively

    Match purchaseMatches, saleMatches;
    
    // TODO replace sorting of inputs with sorting of index arrays
    // then perform operations via index arrays
    // insert current index into purchaseMatches/ saleMatches

    std::vector<size_t> openPurchasesIndices = range(openPurchases.size()),
        openSalesIndices = range(openSales.size());

    // permutation sort DESCENDING => highest buyer is first
    std::sort(openPurchasesIndices.begin(), openPurchasesIndices.end(),
        [&openPurchases](size_t a, size_t b) {
            return openPurchases[a].unitPrice > openPurchases[b].unitPrice;
        }
    );
    // permutation sort ASCENDING => lowest seller is first
    std::sort(openSalesIndices.begin(), openSalesIndices.end(),
        [&openSales](size_t a, size_t b) {
            return openSales[a].unitPrice < openSales[b].unitPrice;
        }
    );

    size_t 
        purchaseScore = openPurchases.size(),
        saleScore = openSales.size();
    size_t purchaseCtr = 0, saleCtr = 0;
    
    while(purchaseScore > 0 || saleScore > 0) {
        if(purchaseScore > saleScore) {
            // find matches for a purchase
            size_t highestBuyer = openPurchasesIndices[purchaseCtr];
            std::vector<size_t> matches = findMatchesForPurchase(openPurchases[highestBuyer], openSales);
            purchaseMatches.insert({highestBuyer, matches});

            // todo check which seller-matches are satisfied => exclude them from future matches
            // todo if highest buyer is now satisfied, exclude him too from findings in the other case
            purchaseCtr++;
            purchaseScore--;
        } else {
            // find matches for sale
            size_t lowestSeller = openSalesIndices[saleCtr];
            std::vector<size_t> matches = findMatchesForSale(openPurchases, openSales[lowestSeller]);
            saleMatches.insert({lowestSeller, matches});

            // todo check which buyer-matches are satisfied => exclude them from future matches
            // todo if lowest seller is now satisfied, exclude him too from findings in the other case
            saleCtr++;
            saleScore--;
        }
    }
}

};