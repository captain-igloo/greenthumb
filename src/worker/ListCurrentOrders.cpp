/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
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

ListCurrentOrders::ListCurrentOrders(
    wxEvtHandler* eventHandler,
    const entity::Market& market,
    const greentop::OrderProjection& orderProjection,
    const uint32_t currentPage) :
    Worker(eventHandler), market(market), orderProjection(orderProjection), currentPage(currentPage) {
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

    greentop::ListCurrentOrdersRequest lcor(std::set<std::string>(), marketIds, orderProjection);

    int pageSize = entity::Config::GetConfigValue<int>(
        entity::Config::KEY_ACCOUNT_PAGE_SIZE,
        dialog::Settings::ACCOUNT_PAGE_SIZE
    );

    lcor.setFromRecord((currentPage - 1) * pageSize);
    lcor.setRecordCount(pageSize);
    return GreenThumb::GetBetfairApi().listCurrentOrders(lcor);
}

}
}
