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

}

wxThread::ExitCode ListCurrentOrders::Entry() {

    greentop::CurrentOrderSummaryReport currentOrderSummaryReport;

    wxLogStatus("Retrieving orders ...");
    try {
        currentOrderSummaryReport = DoListCurrentOrders();
        if (currentOrderSummaryReport.isSuccess()) {
            wxLogStatus("Retrieving orders ... Success");
        } else {
            wxLogStatus("Retrieving orders ... Failed");
        }
    } catch (const std::exception& e) {
        wxLogStatus("Retrieving orders ... Failed: " + _(e.what()));
    }

    wxThreadEvent* event = new wxThreadEvent(LIST_CURRENT_ORDERS);
    event->SetPayload<greentop::CurrentOrderSummaryReport>(currentOrderSummaryReport);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::CurrentOrderSummaryReport ListCurrentOrders::DoListCurrentOrders() {

    std::set<std::string> marketIds;
    marketIds.insert(market.GetMarketCatalogue().getMarketId());
    greentop::OrderProjection op(greentop::OrderProjection::EXECUTABLE);

    greentop::ListCurrentOrdersRequest lcor(std::set<std::string>(), marketIds, op);

    // greentop::ExchangeApi::Exchange exchange =
       //  entity::Exchange::GetExchange(static_cast<entity::Exchange::ExchangeId>(market.GetExchangeId()));

    return GreenThumb::GetBetfairApi().listCurrentOrders(market.GetExchange(), lcor);

}

}
}
