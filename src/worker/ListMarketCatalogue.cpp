/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#include <set>

#include <wx/log.h>

#include "worker/ListMarketCatalogue.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(LIST_MARKET_CATALOGUE, wxThreadEvent);

ListMarketCatalogue::ListMarketCatalogue(wxEvtHandler* eventHandler,
    const std::set<std::string>& marketIds) : Worker(eventHandler), marketIds(marketIds) {
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

    wxThreadEvent* event = new wxThreadEvent(LIST_MARKET_CATALOGUE);
    event->ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event->SetPayload<std::map<std::string, entity::Market>>(betfairMarkets);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;

}

bool ListMarketCatalogue::DoListMarketCatalogue(const std::set<std::string>& marketIdsPage) {

    greentop::sport::MarketFilter filter;
    filter.setMarketIds(marketIdsPage);

    std::set<greentop::sport::MarketProjection> marketProjection;
    marketProjection.insert(greentop::sport::MarketProjection(greentop::sport::MarketProjection::RUNNER_DESCRIPTION));
    marketProjection.insert(greentop::sport::MarketProjection(greentop::sport::MarketProjection::MARKET_START_TIME));
    marketProjection.insert(greentop::sport::MarketProjection(greentop::sport::MarketProjection::MARKET_DESCRIPTION));
    marketProjection.insert(greentop::sport::MarketProjection(greentop::sport::MarketProjection::EVENT));

    greentop::sport::ListMarketCatalogueRequest lmcRequest(filter, marketProjection, greentop::sport::MarketSort(), 1000);

    greentop::sport::ListMarketCatalogueResponse lmcResponse =
        GreenThumb::GetBetfairApi().listMarketCatalogue(lmcRequest);

    if (!TestDestroy() && lmcResponse.isSuccess()) {

        for (auto it = marketIdsPage.begin(); it != marketIdsPage.end(); ++it) {
            entity::Market market;
            betfairMarkets[*it] = market;
        }

        std::vector<greentop::sport::MarketCatalogue> bfMarkets = lmcResponse.getMarketCatalogues();

        for (unsigned i = 0; i < bfMarkets.size(); ++i) {
            entity::Market market;
            market.SetMarketCatalogue(bfMarkets[i]);
            betfairMarkets[bfMarkets[i].getMarketId()] = market;
        }

        return true;
    }
    return false;

}

}
}
