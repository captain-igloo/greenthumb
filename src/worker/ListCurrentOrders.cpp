/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>
#include <wx/log.h>
#include <vector>

#include "dialog/Settings.h"
#include "entity/Config.h"
#include "worker/ListCurrentOrders.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(LIST_CURRENT_ORDERS, wxThreadEvent);

ListCurrentOrders::ListCurrentOrders(wxEvtHandler* eventHandler, entity::Market& market) :
    Worker(eventHandler), market(market) {
}

wxThread::ExitCode ListCurrentOrders::Entry() {
    wxThreadEvent* event = new wxThreadEvent(LIST_CURRENT_ORDERS);

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

    event->SetPayload<greentop::CurrentOrderSummaryReport>(currentOrderSummaryReport);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::CurrentOrderSummaryReport ListCurrentOrders::DoListCurrentOrders() {

    std::set<std::string> marketIds;
    if (market.GetMarketCatalogue().isValid()) {
        marketIds.insert(market.GetMarketCatalogue().getMarketId());
    }
    greentop::OrderProjection op(greentop::OrderProjection::ALL);

    greentop::ListCurrentOrdersRequest lcor(std::set<std::string>(), marketIds, op);

    int pageSize = entity::Config::GetConfigValue<int>(
        entity::Config::KEY_ACCOUNT_PAGE_SIZE,
        dialog::Settings::ACCOUNT_PAGE_SIZE
    );
    lcor.setRecordCount(pageSize);

    return GreenThumb::GetBetfairApi().listCurrentOrders(lcor);
}

}
}
