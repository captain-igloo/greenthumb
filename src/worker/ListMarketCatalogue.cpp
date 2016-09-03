#include <set>

#include <wx/log.h>

#include "worker/ListMarketCatalogue.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(LIST_MARKET_CATALOGUE, wxThreadEvent);

ListMarketCatalogue::ListMarketCatalogue(wxEvtHandler* eventHandler,
    const greentop::Exchange exchange,
    const std::set<std::string>& marketIds) : Worker(eventHandler), exchange(exchange), marketIds(marketIds) {

}

wxThread::ExitCode ListMarketCatalogue::Entry() {

    wxLogStatus("List market catalogue ...");

    try {

        if (marketIds.size() > 50) {
            uint32_t count = 0;
            std::set<std::string> marketIdsPage;
            for (auto it = marketIds.begin(); it != marketIds.end(); ++it) {
                marketIdsPage.insert(*it);
                count++;
                if (count >= 50) {
                    DoListMarketCatalogue(marketIdsPage);
                    marketIdsPage.empty();
                    count = 0;
                }
            }
            if (DoListMarketCatalogue(marketIdsPage)) {
                wxLogStatus("List market catalogue ... Success");
            } else {
                wxLogStatus("List market catalogue ... Failed");
            }
        } else {
            if (DoListMarketCatalogue(marketIds)) {
                wxLogStatus("List market catalogue ... Success");
            } else {
                wxLogStatus("List market catalogue ... Failed");
            }
        }
    } catch (const std::exception& e) {
        wxLogStatus("List market catalogue ... Failed: " + _(e.what()));
    }

    wxThreadEvent* threadEvent = new wxThreadEvent(LIST_MARKET_CATALOGUE);
    threadEvent->ResumePropagation(wxEVENT_PROPAGATE_MAX);
    // threadEvent->SetPayload<std::set<std::string> >(marketIds);
    threadEvent->SetPayload<std::map<std::string, entity::Market>>(betfairMarkets);
    QueueEvent(threadEvent);

    return (wxThread::ExitCode) 0;

}

bool ListMarketCatalogue::DoListMarketCatalogue(const std::set<std::string>& marketIdsPage) {

    greentop::MarketFilter filter;
    filter.setMarketIds(marketIdsPage);

    std::set<greentop::MarketProjection> marketProjection;
    marketProjection.insert(greentop::MarketProjection(greentop::MarketProjection::RUNNER_DESCRIPTION));
    marketProjection.insert(greentop::MarketProjection(greentop::MarketProjection::MARKET_START_TIME));
    marketProjection.insert(greentop::MarketProjection(greentop::MarketProjection::MARKET_DESCRIPTION));

    greentop::ListMarketCatalogueRequest lmcRequest(filter, marketProjection, greentop::MarketSort(), 1000);

    // greentop::Exchange exchange =
       //  entity::Exchange::GetExchange(static_cast<entity::Exchange::ExchangeId>(exchangeId));

    greentop::ListMarketCatalogueResponse lmcResponse =
        GreenThumb::GetBetfairApi().listMarketCatalogue(exchange, lmcRequest);

    if (!TestDestroy() && lmcResponse.isSuccess()) {

        std::vector<greentop::MarketCatalogue> bfMarkets = lmcResponse.getMarketCatalogues();

        for (unsigned i = 0; i < bfMarkets.size(); ++i) {
            entity::Market market;
            market.SetExchange(exchange);
            market.SetMarketCatalogue(bfMarkets[i]);
            betfairMarkets[bfMarkets[i].getMarketId()] = market;
        }

        return true;
    }
    return false;

}

}
}
