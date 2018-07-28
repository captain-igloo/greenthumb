/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>
#include "entity/Market.h"
#include "Util.h"

namespace greenthumb {
namespace entity {

void Market::SetMarketCatalogue(const greentop::MarketCatalogue& marketCatalogue) {
    this->marketCatalogue = marketCatalogue;

    runners.clear();
    for (const greentop::RunnerCatalog& runner : marketCatalogue.getRunners()) {
        if (runner.getSelectionId().isValid()) {
            runners[runner.getSelectionId().getValue()] = runner;
        }
    }

    hasMarketCatalogue = true;
}
const greentop::MarketCatalogue& Market::GetMarketCatalogue() const {
    return marketCatalogue;
}

void Market::SetMarketBook(const greentop::MarketBook& marketBook) {
    this->marketBook = marketBook;

    handicapPages.clear();
    std::vector<PageRunner> handicapPage;
    std::set<int64_t> selectionIdsFound;

    double otherRunnersProfit = 0;

    for (const greentop::Runner& runner : marketBook.getRunners()) {
        if (runner.getSelectionId().isValid()) {
            int64_t selectionId = runner.getSelectionId();
            double handicap = runner.getHandicap().isValid() ? runner.getHandicap().getValue() : 0;

            if (selectionIdsFound.find(selectionId) != selectionIdsFound.end()) {
                // new page
                SetKeyLineIndex(
                    handicapPage,
                    marketBook.getKeyLineDescription(),
                    handicapPages.size()
                );
                handicapPages.push_back(handicapPage);
                handicapPage.clear();
                selectionIdsFound.clear();
                otherRunnersProfit = 0;
            }
            selectionIdsFound.insert(selectionId);
            std::pair<double, double> profitAndLoss = CalculateProfitAndLoss(runner.getMatches());
            for (PageRunner& previousPageRunner : handicapPage) {
                previousPageRunner.profitAndLoss += profitAndLoss.second;
            }
            PageRunner pageRunner(selectionId, handicap, profitAndLoss.first + otherRunnersProfit);
            handicapPage.push_back(pageRunner);
            otherRunnersProfit += profitAndLoss.second;
        }
    }

    if (handicapPage.size() > 0) {
        // add final page
        SetKeyLineIndex(handicapPage, marketBook.getKeyLineDescription(), handicapPages.size());
        handicapPages.push_back(handicapPage);
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

const std::vector<std::vector<PageRunner>>& Market::GetHandicapPages() const {
    return handicapPages;
}

unsigned Market::GetDefaultHandicapIndex() const {
    return defaultHandicapIndex;
}

const std::set<int64_t>& Market::GetSelectionIds() const {
    return selectionIds;
}

void Market::SetKeyLineIndex(
    std::vector<PageRunner> handicapPage,
    greentop::KeyLineDescription keyLineDescription,
    unsigned handicapIndex
) {
    bool found = false;
    if (keyLineDescription.isValid()) {
        for (const greentop::KeyLineSelection& keyLine : keyLineDescription.getKeyLine()) {
            for (const PageRunner& runner : handicapPage) {
                if (DoubleEquals(keyLine.getHandicap(), runner.handicap) && keyLine.getSelectionId() == runner.selectionId) {
                    defaultHandicapIndex = handicapIndex;
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
    }
}

std::pair<double, double> Market::CalculateProfitAndLoss(
    const std::vector<greentop::Match>& matches
) const {
    double thisRunnerProfit = 0;
    double otherRunnersProfit = 0;

    for (const greentop::Match& match : matches) {
        if (match.getPrice().isValid() && match.getSize().isValid()) {
            double backProfit = (match.getPrice().getValue() - 1) * match.getSize().getValue();
            double size = match.getSize().getValue();
            if (match.getSide() == greentop::Side::BACK) {
                thisRunnerProfit += backProfit;
                otherRunnersProfit -= size;
            } else {
                thisRunnerProfit -= backProfit;
                otherRunnersProfit += size;
            }
        }
    }

    return std::pair<double, double>(thisRunnerProfit, otherRunnersProfit);
}

}
}
