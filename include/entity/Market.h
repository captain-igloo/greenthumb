/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#ifndef ENTITY_MARKET_H
#define ENTITY_MARKET_H

#include <greentop/ExchangeApi.h>

namespace greenthumb {
namespace entity {

/**
 * Wraps greentop::MarketCatalogue.
 */
class Market {
    public:

        /**
         * Sets the market catalogue.
         *
         * @param marketCatalogue The market catalogue.
         */
        void SetMarketCatalogue(const greentop::MarketCatalogue& marketCatalogue);

        /**
         * Gets the market catalogue.
         *
         * @return The market catalogue.
         */
        const greentop::MarketCatalogue& GetMarketCatalogue() const;

        /**
         * Sets the market book.
         *
         * @param marketBook The market book.
         */
        void SetMarketBook(const greentop::MarketBook& marketBook);

        /**
         * Gets the market book.
         *
         * @return The market book.
         */
        const greentop::MarketBook& GetMarketBook() const;

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
        const greentop::RunnerCatalog& GetRunner(uint64_t selectionId) const;

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
        const std::vector<std::vector<std::pair<int64_t, double>>>& GetHandicapPages() const;

        /**
         * Returns the index of the handicap page with the most liquidity.
         *
         * @return The default handicap index.
         */
        unsigned GetDefaultHandicapIndex() const;

    private:
        greentop::MarketCatalogue marketCatalogue;
        greentop::MarketBook marketBook;
        std::map<uint64_t, greentop::RunnerCatalog> runners;
        bool hasMarketCatalogue = false;
        std::vector<std::vector<std::pair<int64_t, double>>> handicapPages;
        unsigned defaultHandicapIndex;
};

}
}

#endif // ENTITY_MARKET_H
