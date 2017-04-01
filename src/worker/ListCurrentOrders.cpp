/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>
#include <wx/log.h>
#include <vector>


#include "worker/ListCurrentOrders.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(LIST_CURRENT_ORDERS, wxThreadEvent);

ListCurrentOrders::ListCurrentOrders(wxEvtHandler* eventHandler, entity::Market& market) :
    Worker(eventHandler), market(market) {
    description = "List current orders";
}

wxThread::ExitCode ListCurrentOrders::Entry() {
    wxThreadEvent* event = new wxThreadEvent(LIST_CURRENT_ORDERS);

    greentop::CurrentOrderSummaryReport currentOrderSummaryReport;

    try {
        currentOrderSummaryReport = DoListCurrentOrders();
        if (currentOrderSummaryReport.isSuccess()) {
            event->SetString(_(description) + _(" ... Success"));
        } else {
            event->SetString(_(description) + _(" ... Failed"));
        }
    } catch (const std::exception& e) {
        event->SetString(_(description) + _(" ... Failed: ") + _(e.what()));
    }

    event->SetPayload<greentop::CurrentOrderSummaryReport>(currentOrderSummaryReport);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::CurrentOrderSummaryReport ListCurrentOrders::DoListCurrentOrders() {

    std::set<std::string> marketIds;
    marketIds.insert(market.GetMarketCatalogue().getMarketId());
    greentop::OrderProjection op(greentop::OrderProjection::ALL);

    greentop::ListCurrentOrdersRequest lcor(std::set<std::string>(), marketIds, op);

    return GreenThumb::GetBetfairApi().listCurrentOrders(market.GetExchange(), lcor);
}

}
}
