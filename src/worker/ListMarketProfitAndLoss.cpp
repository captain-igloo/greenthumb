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
    description = "List market profit and loss";
}

wxThread::ExitCode ListMarketProfitAndLoss:: Entry() {
    wxThreadEvent* event = new wxThreadEvent(LIST_MARKET_PROFIT_AND_LOSS);

    greentop::ListMarketProfitAndLossResponse lmpalResp;

    try {
        lmpalResp = DoListMarketProfitAndLoss();

        if (lmpalResp.isSuccess()) {
            event->SetString(_(description) + _(" ... Success"));
        } else {
            event->SetString(_(description) + _(" ... Failed"));
        }

    } catch (const std::exception& e) {
        event->SetString(_(description) + _(" ... Failed: ") + _(e.what()));
    }

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
