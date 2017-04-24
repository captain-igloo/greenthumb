/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <set>

#include "worker/ListMarketProfitAndLoss.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(LIST_MARKET_PROFIT_AND_LOSS, wxThreadEvent);

ListMarketProfitAndLoss::ListMarketProfitAndLoss(wxEvtHandler* eventHandler,
    const greentop::Exchange exchange, const std::string& marketId) :
    Worker(eventHandler), exchange(exchange), marketId(marketId) {
}

wxThread::ExitCode ListMarketProfitAndLoss:: Entry() {
    greentop::ListMarketProfitAndLossResponse lmpalResp;
    wxLogStatus("List market profit and loss ...");
    try {
        lmpalResp = DoListMarketProfitAndLoss();

        if (lmpalResp.isSuccess()) {
            wxLogStatus("List market profit and loss ... Success");
        } else {
            wxLogStatus("List market profit and loss ... Failed");
        }

    } catch (const std::exception& e) {
        wxLogStatus("List market profit and loss ... Failed: " + _(e.what()));
    }

    wxThreadEvent* event = new wxThreadEvent(LIST_MARKET_PROFIT_AND_LOSS);
    event->SetPayload<greentop::ListMarketProfitAndLossResponse>(lmpalResp);

    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::ListMarketProfitAndLossResponse ListMarketProfitAndLoss::DoListMarketProfitAndLoss() {

    std::set<std::string> marketIds;
    marketIds.insert(marketId);

    greentop::ListMarketProfitAndLossRequest lmpalr(marketIds);

    return GreenThumb::GetBetfairApi().listMarketProfitAndLoss(exchange, lmpalr);

}

}
}
