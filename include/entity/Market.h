/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef ENTITY_MARKET_H
#define ENTITY_MARKET_H

#include <greentop/ExchangeApi.h>

namespace greenthumb {
namespace entity {

struct PageRunner {
    PageRunner(
        int64_t selectionId,
        double handicap,
        double profitAndLossAggregate,
        double profitIfWin,
        double profitIfLose
    ) :
        selectionId(selectionId),
        handicap(handicap),
        profitAndLossAggregate(profitAndLossAggregate),
        profitIfWin(profitIfWin),
        profitIfLose(profitIfLose)
    {
    }
    int64_t selectionId;
    double handicap;
    double profitAndLossAggregate;
    double profitIfWin;
    double profitIfLose;
};

/**
 * Wraps greentop::sport::MarketCatalogue.
 */
class Market {
    public:

        /**
         * Sets the market catalogue.
         *
         * @param marketCatalogue The market catalogue.
         */
        void SetMarketCatalogue(const greentop::sport::MarketCatalogue& marketCatalogue);

        /**
         * Gets the market catalogue.
         *
         * @return The market catalogue.
         */
        const greentop::sport::MarketCatalogue& GetMarketCatalogue() const;

        /**
         * Sets the market book.
         *
         * @param marketBook The market book.
         */
        void SetMarketBook(const greentop::sport::MarketBook& marketBook);

        /**
         * Gets the market book.
         *
         * @return The market book.
         */
        const greentop::sport::MarketBook& GetMarketBook() const;

        /**
         * Whether or not the market has a certain runner.
         *
         * @param selectionId The id of the runner.
         * @return True if the market has the runner else false.
         */
        bool HasRunner(uint64_t selectionId) const;

        /**
         * Gets a runner.
         *
         * @param selectionId The id of the runner to get.
         * @throw std::out_of_range If the market doesn't have the specified runner.
         * @return The runner.
         */
        const greentop::sport::RunnerCatalog& GetRunner(uint64_t selectionId) const;

        /**
         * Whether or not the market catalogue has been set.
         *
         * @return True if the market catalogue has been set, else false.
         */
        bool HasMarketCatalogue() const;

        /**
         * Returns information about the market's handicaps; each page consists of a selection id and a handicap.
         *
         * @return The handicap pages.
         */
        const std::vector<std::vector<PageRunner>>& GetHandicapPages() const;

        /**
         * Returns the index of the handicap page with the most liquidity.
         *
         * @return The default handicap index.
         */
        unsigned GetDefaultHandicapIndex() const;

        const std::set<int64_t>& GetSelectionIds() const;

    private:
        greentop::sport::MarketCatalogue marketCatalogue;
        greentop::sport::MarketBook marketBook;
        std::map<uint64_t, greentop::sport::RunnerCatalog> runners;
        bool hasMarketCatalogue = false;
        std::vector <std::vector<PageRunner>> handicapPages;
        unsigned defaultHandicapIndex = 0;
        std::set<int64_t> selectionIds;

        void SetKeyLineIndex(
            std::vector<PageRunner> handicapPage,
            greentop::sport::KeyLineDescription keyLineDescription,
            unsigned handicapIndex
        );

        std::pair<double, double> CalculateProfitAndLoss(
            const std::vector<greentop::sport::Match>& matches
        ) const;
};

}
}

#endif // ENTITY_MARKET_H
