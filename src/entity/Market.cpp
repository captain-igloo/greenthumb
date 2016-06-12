#include "entity/Market.h"

namespace greenthumb {
namespace entity {

void Market::SetExchange(const greentop::Exchange exchange) {
    this->exchange = exchange;
}

const greentop::Exchange Market::GetExchange() const {
    return exchange;
}

void Market::SetMarketCatalogue(const greentop::MarketCatalogue& marketCatalogue) {
    this->marketCatalogue = marketCatalogue;

    runners.clear();

    std::vector<greentop::RunnerCatalog> r = marketCatalogue.getRunners();
    for (unsigned i = 0; i < r.size(); ++i) {
        runners[r[i].getSelectionId()] = r[i];
    }

}
const greentop::MarketCatalogue& Market::GetMarketCatalogue() const {
    return marketCatalogue;
}

bool Market::HasRunner(uint64_t selectionId) const {
    return runners.find(selectionId) != runners.end();
}

const greentop::RunnerCatalog& Market::GetRunner(uint64_t selectionId) const {
    return runners.at(selectionId);
}

}
}
