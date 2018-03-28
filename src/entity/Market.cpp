/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#include "entity/Market.h"

namespace greenthumb {
namespace entity {

void Market::SetMarketCatalogue(const greentop::MarketCatalogue& marketCatalogue) {
    this->marketCatalogue = marketCatalogue;

    runners.clear();
    handicapPages.clear();
    std::vector<std::pair<int64_t, double>> handicapPage;
    std::set<int64_t> selectionIdsFound;

    std::vector<greentop::RunnerCatalog> r = marketCatalogue.getRunners();
    for (unsigned i = 0; i < r.size(); ++i) {
        runners[r[i].getSelectionId()] = r[i];

        if (marketCatalogue.getDescription().getBettingType() == greentop::MarketBettingType::ASIAN_HANDICAP_DOUBLE_LINE) {

            if (selectionIdsFound.find(r[i].getSelectionId()) == selectionIdsFound.end()) {
                selectionIdsFound.insert(r[i].getSelectionId());
                std::pair<int64_t, double> pageRunner(r[i].getSelectionId(), r[i].getHandicap());
                handicapPage.push_back(pageRunner);
            } else {
                // new page
                handicapPages.push_back(handicapPage);
                handicapPage.clear();
                selectionIdsFound.clear();
                selectionIdsFound.insert(r[i].getSelectionId());
                std::pair<int64_t, double> pageRunner(r[i].getSelectionId(), r[i].getHandicap());
                handicapPage.push_back(pageRunner);
            }
        }
    }

    if (handicapPage.size() > 0) {
        // add final page
        handicapPages.push_back(handicapPage);
    }

    hasMarketCatalogue = true;
}
const greentop::MarketCatalogue& Market::GetMarketCatalogue() const {
    return marketCatalogue;
}

void Market::SetMarketBook(const greentop::MarketBook& marketBook) {
    this->marketBook = marketBook;

    if (marketCatalogue.getDescription().getBettingType() == greentop::MarketBettingType::ASIAN_HANDICAP_DOUBLE_LINE) {
        double maxLiquidity = 0;
        double totalLiquidity = 0;
        handicapPages.clear();
        std::vector<std::pair<int64_t, double>> handicapPage;
        std::set<int64_t> selectionIdsFound;

        for (unsigned i = 0; i < marketBook.getRunners().size(); ++i) {
            const greentop::Runner& runner = marketBook.getRunners()[i];

            if (selectionIdsFound.find(runner.getSelectionId()) == selectionIdsFound.end()) {
                selectionIdsFound.insert(runner.getSelectionId());
                std::pair<int64_t, double> pageRunner(runner.getSelectionId(), runner.getHandicap());
                handicapPage.push_back(pageRunner);
            } else {
                // new page
                if (totalLiquidity > maxLiquidity) {
                    maxLiquidity = totalLiquidity;
                    defaultHandicapIndex = handicapPages.size();
                }
                totalLiquidity = 0;
                handicapPages.push_back(handicapPage);
                handicapPage.clear();
                selectionIdsFound.clear();
                selectionIdsFound.insert(runner.getSelectionId());
                std::pair<int64_t, double> pageRunner(runner.getSelectionId(), runner.getHandicap());
                handicapPage.push_back(pageRunner);

            }

            std::vector<greentop::PriceSize> priceSize;
            priceSize = runner.getEx().getAvailableToBack();
            for (unsigned j = 0; j < priceSize.size(); ++j) {
                totalLiquidity += priceSize[j].getSize();
            }
            priceSize = runner.getEx().getAvailableToLay();
            for (unsigned j = 0; j < priceSize.size(); ++j) {
                totalLiquidity += priceSize[j].getSize();
            }
        }

        if (handicapPage.size() > 0) {
            // add final page
            if (totalLiquidity > maxLiquidity) {
                maxLiquidity = totalLiquidity;
                defaultHandicapIndex = handicapPages.size();
            }
            handicapPages.push_back(handicapPage);
        }
    }
}

const greentop::MarketBook& Market::GetMarketBook() const {
    return marketBook;
}

bool Market::HasRunner(uint64_t selectionId) const {
    return runners.find(selectionId) != runners.end();
}

const greentop::RunnerCatalog& Market::GetRunner(uint64_t selectionId) const {
    return runners.at(selectionId);
}

bool Market::HasMarketCatalogue() const {
    return hasMarketCatalogue;
}

const std::vector<std::vector<std::pair<int64_t, double>>>& Market::GetHandicapPages() const {
    return handicapPages;
}

unsigned Market::GetDefaultHandicapIndex() const {
    return defaultHandicapIndex;
}

}
}
