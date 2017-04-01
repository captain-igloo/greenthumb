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
    description = "List market book";
}

wxThread::ExitCode ListMarketBook::Entry() {
    wxThreadEvent* event = new wxThreadEvent(LIST_MARKET_BOOK);

    try {

        if (DoListMarketBook()) {
            event->SetString(_(description) + _(" ... Success"));
        } else {
            event->SetString(_(description) + _(" ... Failed"));
        }

    } catch (const std::exception& e) {
        event->SetString(_(description) + _(" ... Failed: ") + _(e.what()));
    }

    event->SetPayload<greentop::MarketBook>(betfairMarketBook);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;

}

bool ListMarketBook::DoListMarketBook() {

    std::vector<std::string> marketIds;
    marketIds.push_back(market.GetMarketCatalogue().getMarketId());
    std::set<greentop::PriceData> priceData;
    priceData.insert(greentop::PriceData(greentop::PriceData::EX_BEST_OFFERS));
    greentop::PriceProjection priceProjection(priceData);
    greentop::OrderProjection orderProjection(greentop::OrderProjection::EXECUTABLE);
    greentop::MatchProjection matchProjection(greentop::MatchProjection::ROLLED_UP_BY_AVG_PRICE);
    greentop::ListMarketBookRequest listMarketBookRequest(marketIds, priceProjection, orderProjection, matchProjection);

    greentop::ListMarketBookResponse lmbResp = GreenThumb::GetBetfairApi().listMarketBook(
        market.GetExchange(), listMarketBookRequest);

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
