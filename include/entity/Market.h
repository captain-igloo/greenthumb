/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef ENTITY_MARKET_H
#define ENTITY_MARKET_H

#include <greentop/ExchangeApi.h>

namespace greenthumb {
namespace entity {

/**
 * A greentop::MarketCatalogue and its exchange id.
 */
class Market {
    public:

        /**
         * Sets the exchange.
         *
         * @param exchangeId The exchange id.
         */
        void SetExchange(const greentop::Exchange exchange);

        /**
         * Gets the exchange.
         */
        const greentop::Exchange GetExchange() const;

        /**
         * Sets the market catalogue.
         *
         * @param marketCatalogue The market catalogue.
         */
        void SetMarketCatalogue(const greentop::MarketCatalogue& marketCatalogue);
        /**
         * Gets the market catalogue.
         */
        const greentop::MarketCatalogue& GetMarketCatalogue() const;

        /**
         * Whether or not the market has a certain runner.
         *
         * @param selectionId The id of the runner.
         */
        bool HasRunner(uint64_t selectionId) const;

        /**
         * Gets a runner.
         *
         * @param selectionId The id of the runner to get.
         * @throw std::out_of_range If the market doesn't have the specified runner.
         */
        const greentop::RunnerCatalog& GetRunner(uint64_t selectionId) const;

    private:

        unsigned exchangeId;
        greentop::Exchange exchange;
        greentop::MarketCatalogue marketCatalogue;
        std::map<uint64_t, greentop::RunnerCatalog> runners;
};

}
}

#endif // ENTITY_MARKET_H
