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

    wxThreadEvent* threadEvent = new wxThreadEvent(LIST_MARKET_BOOK);
    threadEvent->SetPayload<greentop::MarketBook>(betfairMarketBook);
    QueueEvent(threadEvent);

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
