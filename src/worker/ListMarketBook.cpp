/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#include <set>

#include <wx/log.h>

#include "worker/ListMarketBook.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(LIST_MARKET_BOOK, wxThreadEvent);

ListMarketBook::ListMarketBook(wxEvtHandler* eventHandler,
    const entity::Market& market) :
    Worker(eventHandler), market(market) {
}

wxThread::ExitCode ListMarketBook::Entry() {
    wxLogStatus("List market book ...");
    try {
        if (DoListMarketBook()) {
            wxLogStatus("List market book ... Success");
        } else {
            wxLogStatus("List market book ... Failed");
        }

    } catch (const std::exception& e) {
        wxLogStatus("List market book ... Failed: " + _(e.what()));
    }

    wxThreadEvent* event = new wxThreadEvent(LIST_MARKET_BOOK);
    event->SetPayload<greentop::sport::MarketBook>(betfairMarketBook);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

bool ListMarketBook::DoListMarketBook() {

    std::vector<std::string> marketIds;
    marketIds.push_back(market.GetMarketCatalogue().getMarketId());
    std::set<greentop::sport::PriceData> priceData;
    priceData.insert(greentop::sport::PriceData(greentop::sport::PriceData::EX_BEST_OFFERS));
    greentop::sport::PriceProjection priceProjection(priceData);
    // show "virtual" bets
    priceProjection.setVirtualise(true);
    greentop::sport::OrderProjection orderProjection(greentop::sport::OrderProjection::EXECUTABLE);
    greentop::sport::MatchProjection matchProjection(greentop::sport::MatchProjection::ROLLED_UP_BY_AVG_PRICE);
    greentop::sport::ListMarketBookRequest listMarketBookRequest(marketIds, priceProjection, orderProjection, matchProjection);

    greentop::sport::ListMarketBookResponse lmbResp = GreenThumb::GetBetfairApi().listMarketBook(listMarketBookRequest);

    if (TestDestroy()) {
        return false;
    }

    if (lmbResp.isSuccess()) {
        for (unsigned i = 0; i < lmbResp.getMarketBooks().size(); ++i) {
            betfairMarketBook = lmbResp.getMarketBooks()[i];
            break;
        }
        return true;
    }
    return false;
}


}
}
